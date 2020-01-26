#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <Maths/GXMaths.h>
#include "../GXComponent.h"
#include "Renderer/openGL/Shader/GLShader.h"
#include "Renderer/openGL/Buffers/GLBufferManager.h"
#include "Renderer/openGL/GLRenderer.h"
namespace gx {
	class GX_DLL GXMeshComponent : public GXComponent {
	public:

		GXMeshComponent(const std::shared_ptr<gx::GLBufferManager>& GLBM,GLShader* sh) : GXComponent()
		{
			//TODO make input base shader and use macros to define which shader to use
			this->GLBM = GLBM;
			this->glshader = sh;
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
#ifdef USING_OPENGL
			this->GLDraw();
#endif
		}
	private:
		std::shared_ptr<GLBufferManager> GLBM;
		GLShader* glshader;//points to model owner Shader for now
		// render the mesh
		void GLDraw()
		{
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
				case gx::DIFFUSE:
					name = "material.diffuse" + std::to_string(nDiffuse++);
					break;
				case gx::SPECULAR:
					name = "material.specular" + std::to_string(nSpecular++);
					break;
				case gx::SPECULAR_MAP:
					break;
				case gx::NORMAL:
					break;
				case gx::HEIGHT:
					break;
				case gx::AMBIENT:
					break;
				default:
					break;
				}
				this->glshader->setInt(name.c_str(), i);
				
				textures[i]->use();

			}
			
			
			// draw mesh
			GLBM->use();
			GLRenderer::getInstance().draw(GLBM->getNumberOfElements(), RenderType::GX_TRIANGLES);
			GLBM->stop();

			GLTexture2D::setActiveTexture(0);
		}
	};
}