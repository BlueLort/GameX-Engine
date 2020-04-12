#include "pch.h"
#include "MainScene.h"

namespace gx {
	std::shared_ptr<GXPlane> MainScene::mainPlane;
	void MainScene::init()
	{
		GBuffer.reset(new GXFrameBuffer());
		GBuffer->init(width,height);
		// Setting up the G-Buffer
		GBuffer->addTextureAttachment(GX_COLOR_TEXTURE);
		GBuffer->addTextureAttachment(GX_POSITION_TEXTURE);
		GBuffer->addTextureAttachment(GX_NORMAL_TEXTURE);
		GBuffer->addTextureAttachment(GX_ID_TEXTURE);
		GXuint32 attachments[4] = {
									GBuffer->getAttachmentIndex(GX_POSITION_TEXTURE),
									GBuffer->getAttachmentIndex(GX_NORMAL_TEXTURE),
									GBuffer->getAttachmentIndex(GX_COLOR_TEXTURE) ,
									GBuffer->getAttachmentIndex(GX_ID_TEXTURE) };
		GBuffer->setDrawBuffeers(4,attachments);

		mainSceneBuffer.reset(new GXFrameBuffer());
		mainSceneBuffer->init(width, height);
		mainSceneBuffer->addTextureAttachment(GX_COLOR_TEXTURE);
		lightingPassShader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_DEFERRED);
		lightingPassShader->use();
		lightingPassShader->setInt("gPosition", 0);
		lightingPassShader->setInt("gNormal", 1);
		lightingPassShader->setInt("gAlbedoSpec", 2);

		skydome.reset(new GXSkydomeObject());
		skydome->init("res/models/sphere/spheres.obj");//will wait until it has been imported

		GXGraphicsContext::setViewPort(width, height);
		GXGraphicsContext::setClearColor(0.258f, 0.596f, 0.96f, 1.0f);
		renderingFlags.push_back(GX_CULL_FACE);
		renderingFlags.push_back(GX_DEPTH_TEST);
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
		for (auto& ite : renderingFlags) {
			GXGraphicsContext::enableFlag(ite);
		}
		GXGraphicsContext::clearBufferBits(GX_COLOR_BUFFER_BIT | GX_DEPTH_BUFFER_BIT);

	}

	void MainScene::end()
	{
		GXFrameBuffer::stop();
		for (auto& ite : renderingFlags) {
			GXGraphicsContext::disableFlag(ite);
		}
	}

	int MainScene::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void MainScene::onUpdate(float deltaTime)
	{
		auto ite = sceneModelObjects.begin();
		while(ite!= sceneModelObjects.end()){
			ite->second->update(deltaTime);
			ite++;
		}
		if(mainPlane.get()!=nullptr)mainPlane->update(deltaTime);
		getObjectID();
		skydome->update(deltaTime);
		mainSceneBuffer->use(GX_FBO_RW); // now its time for lightpass
		GXGraphicsContext::clearBufferBits(GX_COLOR_BUFFER_BIT | GX_DEPTH_BUFFER_BIT);
		lightingPassShader->use();
		SceneLightManager::getInstance().setLightValues(lightingPassShader);
		GXTexture2D::setActiveTexture(0);
		GXTexture2D::use(GBuffer->getTextureID(GX_POSITION_TEXTURE));
		GXTexture2D::setActiveTexture(1);
		GXTexture2D::use(GBuffer->getTextureID(GX_NORMAL_TEXTURE));
		GXTexture2D::setActiveTexture(2);
		GXTexture2D::use(GBuffer->getTextureID(GX_COLOR_TEXTURE));
		lightingPassShader->setVec3("viewPos", EditorCamera::getInstance().transform.position);
		quadRenderer->update(deltaTime, lightingPassShader);


	}

	void MainScene::onGUIRender()
	{
		
		ImGui::Begin(name.c_str(), NULL, windowFlags);

		// get mouse loc relative to main window
		selected=ImGui::IsWindowFocused();
	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImGui::Image(reinterpret_cast<void*>(mainSceneBuffer->getTextureID(GX_COLOR_TEXTURE)),windowSize,ImVec2(0,1),ImVec2(1,0));
		ImGui::PopStyleVar(3);
		ImVec2 mops = ImGui::GetMousePos();
		ImVec2 molc = ImGui::GetCursorScreenPos();
		mouseLoc.first = mops.x - molc.x;
		mouseLoc.second = molc.y - mops.y;
		mouseLocNormalized.first = static_cast<float>(mouseLoc.first) / windowSize.x;
		mouseLocNormalized.second = static_cast<float>(mouseLoc.second) / windowSize.y;
	
		ImGui::End();
		GXTexture2D::stop();
		
	}

	GXuint32 MainScene::getObjectID()
	{
		if (!selected || !mouseWasPressed) return 0;
		if (!(mouseLocNormalized.first <= 1.0f && mouseLocNormalized.first >= 0.0f 
			&& mouseLocNormalized.second <= 1.0f && mouseLocNormalized.second >= 0)) return 0;
		GXint32 x = mouseLocNormalized.first * width;
		GXint32 y = mouseLocNormalized.second * height;
		mouseWasPressed = false;
		GXuint32 val;
		GXGraphicsContext::setReadAttachment(GX_COLOR_ATTACHMENT0 + GX_ID_TEXTURE);
		GXGraphicsContext::readPixel(x, y, 1, 1, GX_RED_INTEGER, GX_UNSIGNED_INT, &val);
		GXGraphicsContext::setReadAttachment(GX_NONE);
		if (sceneModelObjects.find(val) == sceneModelObjects.end())return 0;
		return val;
	}

}