#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include "Maths/GXMaths.h"
#include "Components/GXComponent.h"


namespace gx {
	class GX_DLL GXMeshComponent : public GXComponent {
	public:


		//TODO HANDLE GXID PASSING to this component
		GXMeshComponent(uint32_t GXID,const std::shared_ptr<gx::GLBufferManager>& GLBM) : GXComponent(GXID)
		{
			//TODO make input base shader and use macros to define which shader to use
			this->GLBM = GLBM;
		}
		virtual inline const char* getName() const override {
			return "GXMeshComponent";
		}
		virtual inline uint32_t getEventType()const override {
			return GXComponentType::GX_MESH;
		}
		virtual inline uint32_t getEventClass() const override {
			return GXComponentClass::GX_GRAPHICS;
		}
		virtual void destroy()override {
			GLBM->destroy();
		}
		virtual void update(float deltaTime)override {
		}

	private:
		std::shared_ptr<GLBufferManager> GLBM;

		// render the mesh
		void draw(GLShader* glshader,bool isWireFrame) {
			uint32_t nDiffuse = 1;
			uint32_t nSpecular = 1;
			auto textures = GLBM->getTextures();
			for (uint32_t i = 0; i < textures.size(); i++)
			{
				GLTexture2D::setActiveTexture(i);
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
				glshader->setInt(name.c_str(), i);

				textures[i]->use();
			}


			// draw mesh
			GLBM->use();
			GLRenderer::getInstance().draw(GLBM->getNumberOfElements(), RenderType::GX_TRIANGLES,isWireFrame);
			GLBM->stop();
			
			
			GLTexture2D::setActiveTexture(0);
			GLTexture2D::stop();
		}
	};
}