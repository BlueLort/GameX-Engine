#pragma once
#include "Maths/GXMaths.h"
#include "Components/GXComponent.h"
#include "Physics/GXPickingCollider.h"
namespace gx {
	class GX_DLL GXMeshComponent : public GXComponent {
	public:


		//TODO HANDLE GXID PASSING to this component
		GXMeshComponent(GXuint32 GXID,const std::shared_ptr<gx::GXGraphicsBufferManager>& GBM, const std::shared_ptr<gx::GXPickingCollider>& pickingCollider) : GXComponent(GXID)
		{
			//TODO make input base shader and use macros to define which shader to use
			this->graphicsBufferManager = GBM;
			this->gxPickingCollider = pickingCollider;
		}
		GXMeshComponent(GXuint32 GXID, const std::shared_ptr<gx::GXGraphicsBufferManager>& GBM) : GXComponent(GXID)
		{
			//TODO make input base shader and use macros to define which shader to use
			this->graphicsBufferManager = GBM;

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
			graphicsBufferManager->destroy();
		}
		void setOwnerID(GXuint32 GXID) override{
			objectID = GXID;
			this->gxPickingCollider->setOwnerID(GXID);
		}
		virtual void update(float deltaTime)override {
		}
	private:
		std::shared_ptr<GXGraphicsBufferManager> graphicsBufferManager;
		std::shared_ptr<GXPickingCollider> gxPickingCollider;
		// render the mesh
		void draw(GXShader* shader , RenderType type,bool isWireFrame) override{
			GXuint32 nDiffuse = 1;
			GXuint32 nSpecular = 1;
			auto textures = graphicsBufferManager->getTextures();
			for (GXuint32 i = 0; i < textures.size(); i++)
			{
				GXTexture2D::setActiveTexture(i);
				GXTexture2DType type = textures[i]->getType();
				std::string name;
				switch (type)
				{
				case gx::GX_DIFFUSE:
					name = "material.diffuse" + std::to_string(nDiffuse++);
					break;
				case gx::GX_SPECULAR:
					name = "material.specular" + std::to_string(nSpecular++);
					break;
				case gx::GX_SPECULAR_MAP:
					break;
				case gx::GX_NORMAL:
					break;
				case gx::GX_HEIGHT:
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
	};
}