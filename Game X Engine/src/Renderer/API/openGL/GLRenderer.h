#pragma once

#include "Config.h"

#include "Renderer/Renderer.h"

namespace gx {

	class GX_DLL GLRenderer : public Renderer {
	public:
		
		inline static GLRenderer& getInstance() {
			static GLRenderer g;
			return g;
		}
		virtual void init() override;
		virtual void draw(GXuint32 nElements, RenderType type, bool isWireFrame) override;
		virtual inline void setViewPort(GXuint32 Width, GXuint32 Height) override {
			GXGraphicsContext::setViewPort(Width, Height);
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