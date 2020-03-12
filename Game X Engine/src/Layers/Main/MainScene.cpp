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

		skydome.reset(new GXSkydomeObject());
		skydome->GLinit("res/models/sphere/spheres.obj");//will wait until it has been imported

		glViewport(0, 0, width,height);

		glClearColor(0.258f, 0.596f, 0.96f, 1.0f);
		GLFlags.push_back(GL_CULL_FACE);
		GLFlags.push_back(GL_DEPTH_TEST);
		windowFlags = 0;

	}

	void MainScene::destroy()
	{
		GBuffer->destroy();
		mainSceneBuffer->destroy();
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
		lightingPassShader->setInt("gPosition", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, GBuffer->getTextureID(GX_POSITION_ATTACHMENT));
		lightingPassShader->setInt("gNormal",1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, GBuffer->getTextureID(GX_NORMAL_ATTACHMENT));
		lightingPassShader->setInt("gAlbedoSpec", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GBuffer->getTextureID(GX_COLOR_ATTACHMENT));
		lightingPassShader->setVec3("viewPos", EditorCamera::getInstance().transform.position);
		static uint32_t quadVAO = 0;
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			unsigned int quadVBO;
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);


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