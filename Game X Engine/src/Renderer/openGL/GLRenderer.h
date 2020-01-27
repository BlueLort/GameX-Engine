#pragma once

#include "Config.h"
#include "OpenGLContext.h"
#include "../Renderer.h"

namespace gx {

	class GX_DLL GLRenderer : public Renderer {
	public:
		
		inline static GLRenderer& getInstance() {
			static GLRenderer g;
			return g;
		}
		virtual void init() override;
		virtual void draw(uint32_t nElements, RenderType type) override;
		virtual inline void setViewPort(uint32_t Width, uint32_t Height) override {
			gx::OpenGLContext::setViewPort(Width, Height);
		}
		virtual inline void setDepthFunc(DepthFunc func) override {
			glDepthFunc(func);
		}
		virtual inline void setWindingOrder(WindingOrder type) override {
			glFrontFace(type);
		}
	private:
		GLRenderer() {

		}

	};
}