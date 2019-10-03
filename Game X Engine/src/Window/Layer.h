#pragma once
#include "Config.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
namespace gx {
	class Layer;
	struct GX_DLL LayerStack {
		std::vector<std::shared_ptr<Layer> > data;
		int layerStackStart;
		LayerStack() :layerStackStart(0) {}
	};
	class GX_DLL Layer {
	public:
		inline Layer(const std::string& layerName) : name(layerName) {}
		virtual void init() = 0;
		virtual void destroy() = 0;

	

		virtual int onEvent(const gx::event::GXEventType& eventType) = 0;
		virtual void onUpdate(float deltaTime) {}
		virtual void onGUIRender() {}
		inline std::string getName() const { return this->name; }

	protected:
		std::string name ;
	};
}