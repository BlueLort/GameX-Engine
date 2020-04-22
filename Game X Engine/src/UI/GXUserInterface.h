#pragma once
#include "Config.h"
#include "UI/ImGUI_SDLGL/ImGUI_SDLGL.h"

namespace gx {
#ifdef USING_OPENGL
	class GX_DLL GXUserInterface
	{
	public:
		GXUserInterface(const std::string& layerName) :imguiSDLGL(layerName){
		}
		inline void init() { imguiSDLGL.init(); }
		inline void destroy() { imguiSDLGL.destroy(); }
		inline void onGUIRender() { imguiSDLGL.onGUIRender(); }
		inline void startFrame() { imguiSDLGL.startFrame(); }
		inline void render() { imguiSDLGL.render(); }
		inline void endFrame() { imguiSDLGL.endFrame(); }


		//EVENTS
		template<class T>
		inline static GXint32 handleEvent(std::shared_ptr<T>& Event) { return ImGUI_SDLGL::handleEvent(Event); }
	private:
		ImGUI_SDLGL imguiSDLGL;

	};
#endif

}

