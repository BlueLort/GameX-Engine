#pragma once
#include "Config.h"
#include "pch.h"
#include "Renderer//Buffers/GXFrameBufferObject.h"
#include "UI/GXUserInterface.h"
#include "UI/GXGizmo.h"
#include "imgui-filebrowser/imfilebrowser.h"
/*
	Each layer is a framebuffer object that have a texture result asscoiated with it.
*/
namespace gx {
	class GX_DLL Layer {
	public:
		inline Layer(const std::string& layerName) : name(layerName) ,selected(false),isOpen(true){}
		virtual void init() = 0;
		virtual void destroy() = 0;

		virtual void start() = 0;
		virtual void end() = 0;
		virtual GXint32 onEvent(const gx::event::GXEventType& eventType) = 0;
		virtual void onUpdate(GXFloat deltaTime) {}
		virtual void onGUIRender() {}
		inline std::string getName() const { return this->name; }
		virtual inline bool isSelected()const { return selected; }
	protected:
		std::string name;
		bool selected;
		bool isOpen;
	};
	//Priority Queue for Layers to store them by priority highest processed first
	class GX_DLL LayerQueue {
	public:
		std::vector<std::pair<GXuint32, std::shared_ptr<Layer>>> layers;
		inline void add(const std::pair<GXuint32, std::shared_ptr<Layer>>& layer) {
			layers.emplace_back(layer);
			std::sort(layers.begin(), layers.end(), comparator);
		}
		inline void	renderUpdateLayers(GXFloat deltaTime) {
			for (auto& layer : layers) {
				layer.second->start();
				layer.second->onUpdate(deltaTime);
				layer.second->end();
			}
		}
		inline void onGUIRender() {
			for (auto& layer : layers) {
				layer.second->onGUIRender();
			}
		}
		inline void remove(const std::string& name) {
			for (GXint32 i = 0; i < layers.size(); i++) {
				if (layers[i].second->getName() == name) {
					layers.erase(layers.begin() + i);
				}
			}
		}
	private:
		static bool comparator(std::pair<GXuint32, std::shared_ptr<Layer>>& layer1, std::pair<GXuint32, std::shared_ptr<Layer>>& layer2) {
			return layer1.first > layer2.first;//high priority first.
		}
	};
}