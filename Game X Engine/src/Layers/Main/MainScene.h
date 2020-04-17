#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Object/GXObject.h"
#include "Object/GXBackgroundObject/GXSkydomeObject.h"
#include "Terrain/Plane/Plane.h"
#include "IOManagement/IOManager.h"
#include "Shapes/2D/GXQuad.h"
#include "Renderer/Enums.h"
namespace gx {
	class GX_DLL MainScene :public Layer {
	public:
		inline MainScene(const std::string& layerName, GXint32 Width, GXint32 Height) : Layer(layerName),width(Width),height(Height), mouseWasPressed(false){}
		virtual void init()override;
		virtual void destroy()override;
		//IOManagers add the object after it has been instantiated
		inline void addModelObject(std::shared_ptr<GXModelObject>& obj) { sceneModelObjects[obj->getID()]=obj; };
		//does not handle wrong ids.
		inline std::shared_ptr<GXModelObject> getModelObject(GXuint32 GXID){
			auto ite = sceneModelObjects.find(GXID);
			if (ite == sceneModelObjects.end())return nullptr;
			return sceneModelObjects[GXID];
		}
		inline void setPlane(std::shared_ptr<GXPlane>& p) { plane = p; }
		inline std::pair<float, float> getMouseLocNormalized() {
			return mouseLocNormalized;
		}
		inline std::pair<GXint32, GXint32> getMouseLoc() {
			return mouseLoc;
		}
		virtual void start()override;
		virtual void end()override;
		virtual GXint32 onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(float deltaTime)override;
		virtual void onGUIRender()override;
		virtual void mousePressRequest() { mouseWasPressed = true; }
		virtual GXuint32 getObjectID();

	private:
		std::unordered_map<GXuint32,std::shared_ptr<GXModelObject>> sceneModelObjects;
		std::shared_ptr<GXSkydomeObject> skydome;
		std::pair<GXFloat, GXFloat> mouseLocNormalized;
		std::pair<GXint32, GXint32> mouseLoc;
		std::shared_ptr<GXPlane> plane;
		static std::shared_ptr<GXPlane> mainPlane;
		ImGuiWindowFlags windowFlags;
		GXint32 width, height;

		std::vector<GXRendererFlag> renderingFlags;
		std::unique_ptr<GXFrameBuffer> GBuffer;
		GXShader* lightingPassShader = nullptr;
		GXQuad* quadRenderer = nullptr;
		std::unique_ptr<GXFrameBuffer> mainSceneBuffer;
		friend class PlaneEditorLayer;
		bool mouseWasPressed;
	};

}
