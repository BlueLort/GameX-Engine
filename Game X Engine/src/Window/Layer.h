#pragma once
#include "Config.h"
#include "Events/Event.h"
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
		virtual void onEvent(const gx::event::Event& event) = 0;
		virtual void onUpdate(float deltaTime) {}
		virtual void onGUIRender() {}
		inline std::string getName() const { return this->name; }

	protected:
		std::string name ;
	};
}