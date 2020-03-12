#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Object/GXObject.h"
#include "Object/GXBackgroundObject/GXSkydomeObject.h"
#include "Terrain/Plane/Plane.h"
#include "IOManagement/IOManager.h"
namespace gx {
	class GX_DLL MainScene :public Layer {
	public:
		inline MainScene(const std::string& layerName, int Width, int Height) : Layer(layerName),width(Width),height(Height) {}
		virtual void init()override;
		virtual void destroy()override;
		//IOManagers add the object after it has been instantiated
		inline void addModelObject(std::shared_ptr<GXModelObject>& obj) { sceneModelObjects[obj->getID()]=obj; };
		//does not handle wrong ids.
		inline std::shared_ptr<GXModelObject> getModelObject(uint32_t GXID){
			return sceneModelObjects[GXID];
		}
		inline void setPlane(std::shared_ptr<GXPlane>& p) { plane = p; }
		inline std::pair<float, float> getMouseLocNormalized() {
			return mouseLocNormalized;
		}
		virtual void start()override;
		virtual void end()override;
		virtual int onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;

	private:
		std::unordered_map<uint32_t,std::shared_ptr<GXModelObject>> sceneModelObjects;
		std::shared_ptr<GXSkydomeObject> skydome;
		std::pair<float, float> mouseLocNormalized;
		std::shared_ptr<GXPlane> plane;
		static std::shared_ptr<GXPlane> mainPlane;
		ImGuiWindowFlags windowFlags;
		int32_t width, height;
		//just the openGL FBO for now
		std::vector<GLenum> GLFlags;
		std::unique_ptr<GLFrameBuffer> GBuffer;
		GLShader* lightingPassShader;
		std::unique_ptr<GLFrameBuffer> mainSceneBuffer;
		friend class PlaneEditorLayer;
	};

}
