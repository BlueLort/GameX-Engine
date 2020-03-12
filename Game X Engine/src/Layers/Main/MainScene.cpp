#include "pch.h"
#include "MainScene.h"

namespace gx {
	std::shared_ptr<GXPlane> MainScene::mainPlane;
	void MainScene::init()
	{
		GBuffer.reset(new GLFrameBuffer());
		GBuffer->init(width,height);
		//Setting up the G-Buffer
		GBuffer->addTextureAttachment(GX_COLOR_ATTACHMENT);
		GBuffer->addTextureAttachment(GX_POSITION_ATTACHMENT);
		GBuffer->addTextureAttachment(GX_NORMAL_ATTACHMENT);
		uint32_t attachments[3] = { 
									GBuffer->getAttachmentIndex(GX_POSITION_ATTACHMENT),
									GBuffer->getAttachmentIndex(GX_NORMAL_ATTACHMENT),
									GBuffer->getAttachmentIndex(GX_COLOR_ATTACHMENT) };
		GBuffer->setDrawBuffeers(3,attachments);

		mainSceneBuffer.reset(new GLFrameBuffer());
		mainSceneBuffer->init(width, height);
		mainSceneBuffer->addTextureAttachment(GX_COLOR_ATTACHMENT);
		lightingPassShader = GLShaderManager::getShader(GLShaderType::DEFAULT_DEFERRED);
		lightingPassShader->use();
		lightingPassShader->setInt("gPosition", 0);
		lightingPassShader->setInt("gNormal", 1);
		lightingPassShader->setInt("gAlbedoSpec", 2);

		skydome.reset(new GXSkydomeObject());
		skydome->GLinit("res/models/sphere/spheres.obj");//will wait until it has been imported

		glViewport(0, 0, width,height);

		glClearColor(0.258f, 0.596f, 0.96f, 1.0f);
		GLFlags.push_back(GL_CULL_FACE);
		GLFlags.push_back(GL_DEPTH_TEST);
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
		GBuffer->use(GX_FBO_RW); //upload data to the GBUFFER
		GL_CALL(glViewport(0, 0, width, height));//let layers make glviewport according to their resolution
		for (auto& ite : GLFlags) {
			glEnable(ite);
		}
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	}

	void MainScene::end()
	{
		GLFrameBuffer::stop();
		for (auto& ite : GLFlags) {
			glDisable(ite);
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
		skydome->update(deltaTime);
		mainSceneBuffer->use(GX_FBO_RW); //now its time for lightpass
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		lightingPassShader->use();
		SceneLightManager::getInstance().setLightValues(lightingPassShader);
		GLTexture2D::setActiveTexture(0);
		GLTexture2D::use(GBuffer->getTextureID(GX_POSITION_ATTACHMENT));
		GLTexture2D::setActiveTexture(1);
		GLTexture2D::use(GBuffer->getTextureID(GX_NORMAL_ATTACHMENT));
		GLTexture2D::setActiveTexture(2);
		GLTexture2D::use(GBuffer->getTextureID(GX_COLOR_ATTACHMENT));
		lightingPassShader->setVec3("viewPos", EditorCamera::getInstance().transform.position);
		quadRenderer->update(deltaTime, lightingPassShader);

	}

	void MainScene::onGUIRender()
	{
		
		ImGui::Begin(name.c_str(), NULL, windowFlags);

		//get mouse loc relative to main window
		selected=ImGui::IsWindowFocused();
	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImGui::Image(reinterpret_cast<void*>(mainSceneBuffer->getTextureID(GX_COLOR_ATTACHMENT)),windowSize,ImVec2(1,1),ImVec2(0,0));
		ImGui::PopStyleVar(3);
		ImVec2 mops = ImGui::GetMousePos();
		ImVec2 molc = ImGui::GetCursorScreenPos();
		mouseLocNormalized.first = static_cast<float>(mops.x - molc.x) / windowSize.x;
		mouseLocNormalized.second = static_cast<float>(molc.y- mops.y) / windowSize.y;
		
		ImGui::End();
		GLTexture2D::stop();
		
	}

}