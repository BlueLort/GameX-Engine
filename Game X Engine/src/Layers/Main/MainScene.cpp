#include "pch.h"
#include "MainScene.h"

namespace gx {
	void MainScene::init(int width,int height)
	{
		GLFBO.reset(new GLFrameBuffer());
		GLFBO->init(width,height);
		

		glViewport(0, 0, width,height);

		glClearColor(0.258f, 0.596f, 0.96f, 1.0f);
		GLFlags.push_back(GL_CULL_FACE);
		GLFlags.push_back(GL_DEPTH_TEST);
		windowFlags = 0;
	}

	void MainScene::destroy()
	{
		GLFBO->destroy();

	}

	void MainScene::start()
	{
		GLFBO->use();
		for (auto ite : GLFlags) {
			glEnable(ite);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	}

	void MainScene::end()
	{
		GLFrameBuffer::stop();
		for (auto ite : GLFlags) {
			glDisable(ite);
		}
	}

	int MainScene::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void MainScene::onUpdate(float deltaTime)
	{

		for (auto obj : sceneObjects) {
			obj->update(deltaTime);
		}

	}

	void MainScene::onGUIRender()
	{
		
		if (!ImGui::Begin("Scene", NULL, windowFlags)) {
			ImGui::End();
			return;
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Image(reinterpret_cast<void*>(GLFBO->getTextureID()), ImGui::GetContentRegionAvail()
		,ImVec2(1,1),ImVec2(0,0));
		ImGui::PopStyleVar(3);
		ImGui::End();
		
	}

}