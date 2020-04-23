#include "pch.h"
#include "MainScene.h"

namespace gx {
	GXGizmoOperation MainScene::currentGizmoOperation = GXGizmoOperation::GX_TRANSLATE;
	GXGizmoMode MainScene::currentGizmoMode = GXGizmoMode::GX_LOCAL;
	GXBool MainScene::useSnap = false;
	GXFloat MainScene::snap[] = { 1.0f,1.0f,1.0f };
	std::shared_ptr<GXPlane> MainScene::mainPlane;
	constexpr GXFloat OUTLINE_SCALE = 1.1f;
	void MainScene::init()
	{
		GBuffer.reset(new GXFrameBuffer());
		GBuffer->init(width, height);
		// Setting up the G-Buffer
		GBuffer->addTextureAttachment(GX_COLOR_TEXTURE);
		GBuffer->addTextureAttachment(GX_POSITION_TEXTURE);
		GBuffer->addTextureAttachment(GX_NORMAL_TEXTURE);
		GBuffer->addTextureAttachment(GX_ID_TEXTURE);
		GXuint32 attachments[4] = {
									GBuffer->getAttachmentIndex(GX_COLOR_TEXTURE),
									GBuffer->getAttachmentIndex(GX_POSITION_TEXTURE),
									GBuffer->getAttachmentIndex(GX_NORMAL_TEXTURE),
									GBuffer->getAttachmentIndex(GX_ID_TEXTURE) };
		GBuffer->setDrawBuffeers(4, attachments);

		mainSceneBuffer.reset(new GXFrameBuffer());
		mainSceneBuffer->init(width, height);
		mainSceneBuffer->addTextureAttachment(GX_COLOR_TEXTURE);
		lightingPassShader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_DEFERRED);
		lightingPassShader->use();
		lightingPassShader->setInt("gAlbedoSpec", 0);
		lightingPassShader->setInt("gPosition", 1);
		lightingPassShader->setInt("gNormal", 2);
	
		skydome.reset(new GXSkydomeObject());
		skydome->init("res/models/sphere/spheres.obj");//will wait until it has been imported

		debuggingGrid.reset(new GXGrid("Debugging Grid"));
		debuggingGrid->init();


		GXGraphicsContext::setViewPort(width, height);
		GXGraphicsContext::setClearColor(0.258f, 0.596f, 0.96f, 1.0f);
		renderingFlags.push_back(GX_CULL_FACE);
		renderingFlags.push_back(GX_DEPTH_TEST);
		renderingFlags.push_back(GX_STENCIL_TEST);
		GXRenderer::getInstance().setStencilFunc(GX_NOTEQUAL, 1, 0xFF);
		GXRenderer::getInstance().setStencilOperation(GX_KEEP, GX_KEEP, GX_REPLACE);
		windowFlags = 0;
		quadRenderer = new GXQuad();
	}

	void MainScene::destroy()
	{
		GBuffer->destroy();
		mainSceneBuffer->destroy();
		delete quadRenderer;
	}

	void MainScene::start()
	{
		GBuffer->use(GX_FBO_RW); // upload data to the GBUFFER
		GXGraphicsContext::setViewPort(width, height);// let layers make viewport according to their resolution
		for (auto ite : renderingFlags) {
			GXGraphicsContext::enableFlag(ite);
		}
		GXGraphicsContext::clearBufferBits(GX_COLOR_BUFFER_BIT | GX_DEPTH_BUFFER_BIT | GX_STENCIL_BUFFER_BIT);

	}

	void MainScene::end()
	{
		GXFrameBuffer::stop();
		for (auto ite : renderingFlags) {
			GXGraphicsContext::disableFlag(ite);
		}
	}

	int MainScene::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void MainScene::onUpdate(GXFloat deltaTime)
	{
		//Render selected object
		GXRenderer::getInstance().setStencilFunc(GX_ALWAYS, 1, 0xFF);
		GXRenderer::getInstance().setStencilMask(0xFF);
		updateObjects(deltaTime);
		selectObjectUnderCursor();
		updatePlane(deltaTime);
		GXRenderer::getInstance().setStencilMask(0x00);
		skydome->update(deltaTime); //rendering to the COLOR_TEXTURE in the framebuffer
		debuggingGrid->update(deltaTime);//TODO MAKE IT POSSIBLE TO HIDE THE GRID
		updateSelectedObject(deltaTime);//TODO change outline approach [benchmark mathematical dilation]
		
		mainSceneBuffer->use(GX_FBO_RW); // now its time for lightpass
		//disable stencil and depth testing during lighting pass
		GXGraphicsContext::disableFlag(GX_DEPTH_TEST);
		GXGraphicsContext::disableFlag(GX_STENCIL_TEST);
		GXGraphicsContext::clearBufferBits(GX_COLOR_BUFFER_BIT);
		lightingPassShader->use();
		SceneLightManager::getInstance().setLightValues(lightingPassShader);
		GXTexture2D::setActiveTexture(0);
		GXTexture2D::use(GBuffer->getTextureID(GX_COLOR_TEXTURE));
		GXTexture2D::setActiveTexture(1);
		GXTexture2D::use(GBuffer->getTextureID(GX_POSITION_TEXTURE));
		GXTexture2D::setActiveTexture(2);
		GXTexture2D::use(GBuffer->getTextureID(GX_NORMAL_TEXTURE));

		lightingPassShader->setVec3("viewPos", EditorCamera::getInstance().transform.position);
		
		quadRenderer->update(deltaTime, lightingPassShader);


	}

	void MainScene::onGUIRender()
	{
		manipulateSelectedObject();//Another window if object selected
		ImGui::Begin(name.c_str(), NULL, windowFlags);
		// get mouse loc relative to main window
		selected = ImGui::IsWindowFocused();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImGui::Image(reinterpret_cast<void*>(mainSceneBuffer->getTextureID(GX_COLOR_TEXTURE)), windowSize, ImVec2(0, 1), ImVec2(1, 0));
		drawGizmoOnSelectedObject();//Draw Guizmo on selected Object
		ImGui::PopStyleVar(3);
		ImVec2 mops = ImGui::GetMousePos();
		ImVec2 molc = ImGui::GetCursorScreenPos();
		mouseLoc.first = static_cast<GXint32>(mops.x - molc.x);
		mouseLoc.second = static_cast<GXint32>(molc.y - mops.y);
		mouseLocNormalized.first = static_cast<GXFloat>(mouseLoc.first) / windowSize.x;
		mouseLocNormalized.second = static_cast<GXFloat>(mouseLoc.second) / windowSize.y;

		ImGui::End();
		GXTexture2D::stop();
		
	}

	GXuint32 MainScene::selectObjectUnderCursor()
	{
		if (!selected || !mouseWasPressed) return 0;
		if (GXGizmo::isOver()) {
			mouseWasPressed = false;
			return 0;
		}
		if (!(mouseLocNormalized.first <= 1.0f && mouseLocNormalized.first >= 0.0f
			&& mouseLocNormalized.second <= 1.0f && mouseLocNormalized.second >= 0)) return 0;
		GXint32 x = static_cast<GXint32>(mouseLocNormalized.first * width);
		GXint32 y = static_cast<GXint32>(mouseLocNormalized.second * height);
		mouseWasPressed = false;
		GXuint32 val;
		GXGraphicsContext::setReadAttachment(GX_COLOR_ATTACHMENT0 + GX_ID_TEXTURE);
		GXGraphicsContext::readPixel(x, y, 1, 1, GX_RED_INTEGER, GX_UNSIGNED_INT, &val);
		GXGraphicsContext::setReadAttachment(GX_NONE);
		auto ite = sceneModelObjects.find(val);
		if (ite == sceneModelObjects.end()) {
			if (selectedObject != nullptr) {
				selectedObject.reset();
			}
			return 0;
		}
		selectedObject = ite->second;
		return val;
	}

	void MainScene::updateObjects(GXFloat deltaTime)
	{
		auto ite = sceneModelObjects.begin();
		while (ite != sceneModelObjects.end()) {
			ite->second->update(deltaTime);
			ite++;
		}
	}

	GXBool MainScene::updatePlane(GXFloat deltaTime)
	{
		if (mainPlane.get() != nullptr) {
			mainPlane->update(deltaTime);
			return true;
		}
		return false;
	}

	GXBool MainScene::updateSelectedObject(GXFloat deltaTime)
	{
		if (selectedObject == nullptr)return false;
		//Render selected object outline
		GXShader* objShader = selectedObject->getShader();
		selectedObject->setShader(GXShaderManager::getShader(GXCompiledShader::DEFAULT_OUTLINE));
		GXRenderer::getInstance().setStencilFunc(GX_NOTEQUAL, 1, 0xFF);
		GXRenderer::getInstance().setStencilMask(0x00);
		GXGraphicsContext::disableFlag(GX_DEPTH_TEST);
		selectedObject->update(deltaTime);
		selectedObject->setShader(objShader);//return to object shader
		GXRenderer::getInstance().setStencilMask(0xFF);
		GXGraphicsContext::enableFlag(GX_DEPTH_TEST);
		return true;
	}

	void MainScene::manipulateSelectedObject()
	{
		ImGui::Begin("Editor");
		//SET initial modes
		
		if (selectedObject == nullptr) {
			ImGui::Text("No Object Selected");
			GXGizmo::setEnabled(false);
		}
		else {
			
			//camera & selected object are needed
			//Hotkeys to Change operation
			if (InputManager::getInstance().isPressed(gx::event::key::GXK_E)) {
				currentGizmoOperation = GXGizmoOperation::GX_TRANSLATE;
			}
			if (InputManager::getInstance().isPressed(gx::event::key::GXK_R)) {
				currentGizmoOperation = GXGizmoOperation::GX_ROTATE;
			}
			if (InputManager::getInstance().isPressed(gx::event::key::GXK_T)) {
				currentGizmoOperation = GXGizmoOperation::GX_SCALE;
			}
			//Change operations using UI	
			if (ImGui::RadioButton("Translate", currentGizmoOperation == GXGizmoOperation::GX_TRANSLATE))
				currentGizmoOperation = GXGizmoOperation::GX_TRANSLATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", currentGizmoOperation == GXGizmoOperation::GX_ROTATE))
				currentGizmoOperation = GXGizmoOperation::GX_ROTATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", currentGizmoOperation == GXGizmoOperation::GX_SCALE))
				currentGizmoOperation = GXGizmoOperation::GX_SCALE;

			//Change object using UI
			GXFloat* position = GXMaths::getDataPtr(selectedObject->transform.position);
			GXFloat* rotation = GXMaths::getDataPtr(selectedObject->transform.rotation);
			GXFloat* scale = GXMaths::getDataPtr(selectedObject->transform.scale);
			ImGui::InputFloat3("Position", position, 3);
			ImGui::InputFloat3("Rotation", rotation, 3);
			ImGui::InputFloat3("Scale", scale, 3);
			CLAMP(scale[0], 0.001f, FLT_MAX);
			CLAMP(scale[1], 0.001f, FLT_MAX);
			CLAMP(scale[2], 0.001f, FLT_MAX);
			selectedObject->transform.position = GXVec3(position[0], position[1], position[2]);
			selectedObject->transform.rotation = GXVec3(rotation[0], rotation[1], rotation[2]);
			selectedObject->transform.scale = GXVec3(scale[0], scale[1], scale[2]);
			
			//Change Mode using UI
			if (currentGizmoOperation != GXGizmoOperation::GX_SCALE)
			{
				if (ImGui::RadioButton("Local", currentGizmoMode == GXGizmoMode::GX_LOCAL))
					currentGizmoMode = GXGizmoMode::GX_LOCAL;
				ImGui::SameLine();
				if (ImGui::RadioButton("World", currentGizmoMode == GXGizmoMode::GX_WORLD))
					currentGizmoMode = GXGizmoMode::GX_WORLD;
			}

			//change snap or not using UI
			ImGui::Checkbox("##", &useSnap);
			ImGui::SameLine();
			
			switch (currentGizmoOperation)
			{
			case GXGizmoOperation::GX_TRANSLATE:
				ImGui::InputFloat3("Snap", snap);
				break;
			case  GXGizmoOperation::GX_ROTATE:
				ImGui::InputFloat("Angle Snap", snap);
				break;
			case  GXGizmoOperation::GX_SCALE:
				ImGui::InputFloat("Scale Snap", snap);
				break;
			}
			ImGui::Checkbox("Wire Frame", &selectedObject->isWireFrame);
			
			//TODO HANDLE THIS WITH A MEMORY MANAGER
			delete[] position;
			delete[] rotation;
			delete[] scale;
		}
		ImGui::End();
	}

	void MainScene::drawGizmoOnSelectedObject()
	{
		if (selectedObject == nullptr)return;
		GXGizmo::setOrthographic(false);
		GXGizmo::setEnabled(true);
		GXFloat* objMatrix = GXMaths::getDataPtr(selectedObject->transform.getModel());
		
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 windowPos = ImGui::GetWindowPos();
		GXGizmo::setRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);
		GXGizmo::manipulate(GXPtr(EditorCamera::getInstance().getViewMatrix()), GXPtr(EditorCamera::getInstance().getProjectionMatrix()), currentGizmoOperation, currentGizmoMode, objMatrix, NULL, useSnap ? snap : NULL);
		GXFloat pos[3], rot[3], scl[3];
		GXGizmo::decomposeMatrix(objMatrix, pos, rot, scl);
		selectedObject->transform.position = GXVec3(pos[0], pos[1], pos[2]);
		selectedObject->transform.rotation = GXVec3(rot[0], rot[1],rot[2]);
		selectedObject->transform.scale = GXVec3(scl[0], scl[1], scl[2]);
		//TODO HANDLE THIS WITH A MEMORY MANAGER
		delete[] objMatrix;
	}


}