#pragma once
#include "Config.h"
#include "pch.h"
#include "Renderer/openGL/Buffers/GLFrameBufferObject.h"
/*
	Each layer is a framebuffer object that have a texture result asscoiated with it.
*/
namespace gx {
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
		std::string name;

		GLFrameBuffer GLFBO;//just the openGL FBO for now
	};
	//Priority Queue for Layers to store them by priority highest processed first
	class GX_DLL LayerQueue {
		std::vector<std::pair<uint32_t, std::shared_ptr<Layer>>> data;
		int layerQueueStart;
		inline void add(const std::pair<uint32_t, std::shared_ptr<Layer>>& layer) {
			data.emplace_back(layer);
			std::sort(data.begin(), data.end(), comparator);
		}
		inline void remove(const std::string& name) {
			for (int i = 0; i < data.size(); i++) {
				if (data[i].second->getName() == name) {
					data.erase(data.begin() + i);
				}
			}
		}
		LayerQueue() :layerQueueStart(0) {}
	private:
		static bool comparator(std::pair<uint32_t, std::shared_ptr<Layer>>& layer1, std::pair<uint32_t, std::shared_ptr<Layer>>& layer2) {
			return layer1.first > layer2.first;//hight priority first.
		}
	};
}