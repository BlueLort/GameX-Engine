#pragma once
#include "Maths/GXMaths.h"
#include "Components/GXComponent.h"
#include "Physics/GXPickingCollider.h"
#include "UI/ImGUI_SDLGL/ImGUI_SDLGL.h"
#include "imgui-filebrowser/imfilebrowser.h"
#include "stb_image/stb_image.h"
namespace gx {

	struct GXMeshMaterialController {
		GXuint32 diffuse, specular, normal,componentID;
		GXint32 selected;
		ImGui::FileBrowser fileDialog;
		std::shared_ptr<GXGraphicsBufferManager> meshGBM;
		GXMeshMaterialController(): diffuse(0), specular(0), normal(0),componentID(0), selected(0) {
			fileDialog.SetTitle("File Browser");
			fileDialog.SetTypeFilters({ ".jpg", ".png" });
		}
		void setID(GXuint64 id) { componentID = id; }
		void setDiffuse(GXuint32 id) { diffuse = id; }
		void setSpecular(GXuint32 id) { specular = id; }
		void setNormal(GXuint32 id) { normal = id; }
		void setGBM(const std::shared_ptr<GXGraphicsBufferManager>& GBM) {
			meshGBM = GBM;
		}
		void onGUIRender() {
			ImGui::Begin("Editor");
			ImGui::Separator();
			ImGui::Text(std::string("Mesh Component " + std::to_string(componentID)).c_str());
			ImVec2 windowSize = ImVec2(128.0f,128.0f);
			ImGui::Text(std::string("Diffuse Texture#" + std::to_string(componentID)).c_str());
			ImGui::Image(reinterpret_cast<void*>(diffuse), windowSize, ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::Button(std::string("Import##diffuse" + std::to_string(componentID)).c_str())) {
				fileDialog.Open();
				selected = GX_DIFFUSE;
			}
			ImGui::Text(std::string("Specular Texture#" + std::to_string(componentID)).c_str());
			ImGui::Image(reinterpret_cast<void*>(specular), windowSize, ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::Button(std::string("Import##specular" + std::to_string(componentID)).c_str())) {
				fileDialog.Open();
				selected = GX_SPECULAR;
			}
			ImGui::Text(std::string("Normal Texture#" + std::to_string(componentID)).c_str());
			ImGui::Image(reinterpret_cast<void*>(normal), windowSize, ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::Button(std::string("Import##normal" + std::to_string(componentID)).c_str())) {
				fileDialog.Open();
				selected = GX_NORMAL;
			}
			fileDialog.Display();
			if (fileDialog.HasSelected())
			{
				GXint32 width, height, nChannels;
				GXuint8* img = stbi_load(fileDialog.GetSelected().string().c_str(), &width, &height, &nChannels, 0);
				std::shared_ptr<GXTexture2D> tex;
				tex.reset(new GXTexture2D());
				if (img) {
					
					GXTexture2DColorType textype = (nChannels == 4 ? GX_RGBA : GX_RGB);
					tex->init(img, width, height, textype, textype, static_cast<GXTexture2DType>(selected), GXTexture2DFilteringMethod::GX_LINEAR, GX_UNSIGNED_BYTE);
					GXE_DEBUG("Texture2D imported successfully\nPath: {0}", fileDialog.GetSelected().string().c_str());
					switch (selected)
					{
					case GX_DIFFUSE:diffuse = tex->getID(); break;
					case GX_SPECULAR:specular = tex->getID(); break;
					case GX_NORMAL:normal = tex->getID(); break;
					}
				}
				else {
					GXE_ERROR("Failed to import Texture2D\nPath: {0}", fileDialog.GetSelected().string().c_str());
				}
				auto textures = meshGBM->getTextures();
				for (GXuint32 i = 0; i < textures.size(); i++)
				{
					GXTexture2DType type = textures[i]->getType();
					switch (type)
					{
					case gx::GX_DIFFUSE:
						textures[i]->init(diffuse, GX_DIFFUSE);
						if (selected == GX_DIFFUSE)selected = 0;
						break;
					case gx::GX_SPECULAR:
						textures[i]->init(specular, GX_SPECULAR);
						if (selected == GX_SPECULAR)selected = 0;
						break;
					case gx::GX_NORMAL:
						textures[i]->init(normal, GX_NORMAL);
						if (selected == GX_NORMAL)selected = 0;
						break;
					}
				}
				if (selected != 0) {
					meshGBM->addTexture(tex);
					selected = 0;
				}
				fileDialog.ClearSelected();
			}
			ImGui::End();
			GXTexture2D::stop();
		}


	};
	class GX_DLL GXMeshComponent : public GXComponent {
	public:


		//TODO HANDLE GXID PASSING to this component
		GXMeshComponent(GXuint32 GXID,const std::shared_ptr<gx::GXGraphicsBufferManager>& GBM, const std::shared_ptr<gx::GXPickingCollider>& pickingCollider) : GXComponent(GXID)
		{
			//TODO make input base shader and use macros to define which shader to use
			this->graphicsBufferManager = GBM;
			this->gxPickingCollider = pickingCollider;
			this->matController.setID(this->componentID);
			this->matController.setGBM(GBM);
		}
		GXMeshComponent(GXuint32 GXID, const std::shared_ptr<gx::GXGraphicsBufferManager>& GBM) : GXComponent(GXID)
		{
			//TODO make input base shader and use macros to define which shader to use
			this->graphicsBufferManager = GBM;
			this->matController.setID(this->componentID);
			this->matController.setGBM(GBM);
		}

		virtual inline const char* getName() const override {
			return "GXMeshComponent";
		}
		virtual inline GXuint32 getEventType() const override {
			return GXComponentType::GX_MESH;
		}
		virtual inline GXuint32 getEventClass() const override {
			return GXComponentClass::GX_GRAPHICS;
		}
		virtual void destroy()override {
			//graphicsBufferManager->destroy(); //mesh components are currently cached into the iomanager so i can't delete their GBM
		}
		void setOwnerID(GXuint32 GXID) override{
			objectID = GXID;
			this->gxPickingCollider->setOwnerID(GXID);
		}
		virtual void update(float deltaTime)override {
		}
		void onGUIRender() override {
			matController.onGUIRender();
		}
	private:
		std::shared_ptr<GXGraphicsBufferManager> graphicsBufferManager;
		std::shared_ptr<GXPickingCollider> gxPickingCollider;
		GXMeshMaterialController  matController;
		// render the mesh
		void draw(GXShader* shader , RenderType type,bool isWireFrame) override{
			auto textures = graphicsBufferManager->getTextures();
			for (GXuint32 i = 0; i < textures.size(); i++)
			{
				GXTexture2D::setActiveTexture(i);
				GXTexture2DType type = textures[i]->getType();
				std::string name;
				switch (type)
				{
				case gx::GX_DIFFUSE:
					name = "material.diffuse";
					matController.setDiffuse(textures[i]->getID());
					break;
				case gx::GX_SPECULAR:
					name = "material.specular";
					matController.setSpecular(textures[i]->getID());
					break;
				case gx::GX_SPECULAR_MAP:
					break;
				case gx::GX_NORMAL:
					name = "material.normal";
					matController.setNormal(textures[i]->getID());
					break;
				case gx::GX_HEIGHT:
					name = "hmap"; //used with plane only up till now
					break;
				case gx::GX_AMBIENT:
					break;
				default:
					break;
				}
				shader->setInt(name.c_str(), i);

				textures[i]->use();
			}

			// draw mesh
			graphicsBufferManager->use();
			GXRenderer::getInstance().draw(graphicsBufferManager->getNumberOfElements(), type,isWireFrame);
			graphicsBufferManager->stop();
			
			
			GXTexture2D::setActiveTexture(0);
			GXTexture2D::stop();
		}
		void draw(GXShader* shader, RenderType type, bool isWireFrame, GXuint32 nInstances) override {
			auto textures = graphicsBufferManager->getTextures();
			for (GXuint32 i = 0; i < textures.size(); i++)
			{
				GXTexture2D::setActiveTexture(i);
				GXTexture2DType type = textures[i]->getType();
				std::string name;
				switch (type)
				{
				case gx::GX_DIFFUSE:
					name = "material.diffuse";
					matController.setDiffuse(textures[i]->getID());
					break;
				case gx::GX_SPECULAR:
					name = "material.specular";
					matController.setSpecular(textures[i]->getID());
					break;
				case gx::GX_SPECULAR_MAP:
					break;
				case gx::GX_NORMAL:
					name = "material.normal";
					matController.setNormal(textures[i]->getID());
					break;
				case gx::GX_HEIGHT:
					name = "hmap"; //used with plane only up till now
					break;
				case gx::GX_AMBIENT:
					break;
				default:
					break;
				}
				shader->setInt(name.c_str(), i);

				textures[i]->use();
			}

			// draw mesh
			graphicsBufferManager->use();
			GXRenderer::getInstance().draw(graphicsBufferManager->getNumberOfElements(), type, isWireFrame,nInstances);
			graphicsBufferManager->stop();


			GXTexture2D::setActiveTexture(0);
			GXTexture2D::stop();
		}
	};
}