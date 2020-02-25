#include "pch.h"
#include "MainScene.h"

namespace gx {
	std::shared_ptr<GXPlane> MainScene::mainPlane;
	void MainScene::init()
	{
		GLFBO.reset(new GLFrameBuffer());
		GLFBO->init(width,height);
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
		GLFBO->destroy();

	}

	void MainScene::start()
	{
		GLFBO->use();
		glViewport(0, 0, width, height);//let layers make glviewport according to their resolution
		for (auto& ite : GLFlags) {
			glEnable(ite);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

		for (int i = 0; i < sceneModelObjects.size();i++) {
			sceneModelObjects[i]->update(deltaTime);
		}
		if(mainPlane.get()!=nullptr)mainPlane->update(deltaTime);
		skydome->update(deltaTime);
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
		ImGui::Image(reinterpret_cast<void*>(GLFBO->getTextureID()),windowSize,ImVec2(1,1),ImVec2(0,0));
		ImGui::PopStyleVar(3);
		ImVec2 mops = ImGui::GetMousePos();
		ImVec2 molc = ImGui::GetCursorScreenPos();
		mouseLocNormalized.first = static_cast<float>(mops.x - molc.x) / windowSize.x;
		mouseLocNormalized.second = static_cast<float>(molc.y- mops.y) / windowSize.y;
		
		ImGui::End();
		GLTexture2D::stop();
		
	}

}