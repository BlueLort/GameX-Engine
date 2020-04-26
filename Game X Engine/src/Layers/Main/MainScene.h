#pragma once
#include "Config.h"
#include "Layers/Layer.h"
#include "Object/GXObject.h"
#include "Object/GXBackgroundObject/GXSkydomeObject.h"
#include "Object/Debug/GXGrid.h"
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
		inline void addModelObject(std::shared_ptr<GXModelObject>& obj) {
			if (obj->isReady)
				sceneModelObjects[obj->getID()] = obj;
			else {
				modelObjectRequests.push_back(obj);
			}			
		};
		//does not handle wrong ids.
		inline std::shared_ptr<GXModelObject> getModelObject(GXuint32 GXID){
			auto ite = sceneModelObjects.find(GXID);
			if (ite == sceneModelObjects.end())return nullptr;
			return sceneModelObjects[GXID];
		}
		inline void setPlane(std::shared_ptr<GXPlane>& p) { plane = p; }
		inline std::pair<GXFloat,GXFloat> getMouseLocNormalized() {
			return mouseLocNormalized;
		}
		inline std::pair<GXint32, GXint32> getMouseLoc() {
			return mouseLoc;
		}
		virtual void start()override;
		virtual void end()override;
		virtual GXint32 onEvent(const gx::event::GXEventType& eventType)override;
		virtual void onUpdate(GXFloat deltaTime)override;
		virtual void onGUIRender()override;
		virtual void mousePressRequest() { if(selected)mouseWasPressed = true; }
	protected:
		virtual GXuint32 selectObjectUnderCursor();
		virtual void updateObjects(GXFloat deltaTime);
		virtual GXBool updatePlane(GXFloat deltaTime);
		virtual GXBool updateSelectedObject(GXFloat deltaTime);
		virtual void manipulateSelectedObject();
		virtual void displayHierarchy();
		virtual void drawGizmoOnSelectedObject();

	private:
		//GIZMOS RELATED FUNCTIONS
		static GXGizmoOperation currentGizmoOperation;
		static GXGizmoMode currentGizmoMode;
		static GXBool useSnap;
		static GXFloat snap[];
		ImGui::FileBrowser fileDialog;//file browser using imfilebrowser
		std::vector<std::shared_ptr<GXModelObject>> modelObjectRequests;

		std::unordered_map<GXuint32,std::shared_ptr<GXModelObject>> sceneModelObjects;
		std::shared_ptr<GXModelObject> selectedObject;
		std::shared_ptr<GXSkydomeObject> skydome;
		std::shared_ptr<GXGrid> debuggingGrid;
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

		inline void updateModelRequests() {
			if (!modelObjectRequests.empty()) {
				for (GXint32 i = 0; i < modelObjectRequests.size(); i++) {
					if (modelObjectRequests[i]->isReady) {
						modelObjectRequests[i]->fixComponentsID();
						sceneModelObjects[modelObjectRequests[i]->getID()] = modelObjectRequests[i];
						modelObjectRequests.erase(modelObjectRequests.begin() + i);
						i--;
					}
				}
			}
		}
	};

}
