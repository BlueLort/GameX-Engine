#pragma once
#include "Config.h"
#include "pch.h"
#include "Renderer/openGL/Buffers/GLFrameBufferObject.h"
#include "UI/ImGUI_SDLGL.h"
/*
	Each layer is a framebuffer object that have a texture result asscoiated with it.
*/
namespace gx {
	class GX_DLL Layer {
	public:
		inline Layer(const std::string& layerName) : name(layerName) {}
		virtual void init(int width, int height) = 0;
		virtual void destroy() = 0;

		virtual void start() = 0;
		virtual void end() = 0;
		virtual int onEvent(const gx::event::GXEventType& eventType) = 0;
		virtual void onUpdate(float deltaTime) {}
		virtual void onGUIRender() {}
		inline std::string getName() const { return this->name; }

	protected:
		std::string name;
		//just the openGL FBO for now
		std::vector<GLenum> GLFlags;
		std::unique_ptr<GLFrameBuffer> GLFBO;
	};
	//Priority Queue for Layers to store them by priority highest processed first
	class GX_DLL LayerQueue {
	public:
		std::vector<std::pair<uint32_t, std::shared_ptr<Layer>>> layers;
		inline void add(const std::pair<uint32_t, std::shared_ptr<Layer>>& layer) {
			layers.emplace_back(layer);
			std::sort(layers.begin(), layers.end(), comparator);
		}
		inline void	renderUpdateLayers(float deltaTime) {
			for (auto layer : layers) {
				layer.second->start();
				layer.second->onUpdate(deltaTime);
				layer.second->end();
			}
		}
		inline void onGUIRender() {
			for (auto layer : layers) {
				layer.second->onGUIRender();
			}
		}
		//TODO MAKE THE REST OF LAYER FUNCs
		inline void remove(const std::string& name) {
			for (int i = 0; i < layers.size(); i++) {
				if (layers[i].second->getName() == name) {
					layers.erase(layers.begin() + i);
				}
			}
		}
	private:
		static bool comparator(std::pair<uint32_t, std::shared_ptr<Layer>>& layer1, std::pair<uint32_t, std::shared_ptr<Layer>>& layer2) {
			return layer1.first > layer2.first;//high priority first.
		}
	};
}