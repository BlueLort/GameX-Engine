#pragma once
#include "Config.h"
#include "pch.h"
#include "Camera/Camera.h"
//TODO ADD CONTROL LOGIC
namespace gx {
	/*Most logic copied from learnopengl.com ...just a change in the convention.*/

	// Default camera values
	constexpr GXFloat YAW = -90.0f;
	constexpr GXFloat PITCH = 0.0f;
	constexpr GXFloat SPEED = 1.0f;
	constexpr GXFloat SENSITIVITY = 1.25f;
	constexpr GXFloat ZOOM = 45.0f;
	constexpr GXFloat MOVEMENT_SPEED = 16.0f;
	// Default Projection values
	constexpr GXFloat ASPECT_RATIO = 1024.0f / 768.0f;
	constexpr GXFloat zNEAR = 0.1f;
	constexpr GXFloat zFAR = 1000.0f;
	class GX_DLL EditorCamera:public Camera
	{
	public:
		GXFloat zNear, zFar, AR;
		static EditorCamera& getInstance(){ 
			static EditorCamera ecam;
			return ecam;
		}
		void init(const GXVec3& position = GXVec3(0.0f, 10.0f, 10.0f), const GXVec3& up = GXVec3(0.0f, 1.0f, 0.0f), GXFloat yaw = YAW, GXFloat pitch = PITCH, GXFloat AR=ASPECT_RATIO, GXFloat zNear=zNEAR, GXFloat zFar=zFAR);

		virtual void processKeyboard(CameraMovement direction,GXFloat deltaTime)override;
		virtual void processMouseMovement(GXFloat xoffset, GXFloat yoffset, GXBool constrainPitch = true) override;
		virtual void processMouseScroll(GXFloat yoffset) override;
		virtual inline GXMat4 getPVMatrix() const { return PVMatrix; }
		virtual inline GXMat4 getViewMatrix() const
		{
			return view;
		}
		virtual inline void setViewMatrix(GXFloat* viewMat) {
			GXint32 k = 0;
			for (GXint32 i = 0; i < 4; i++) {
				for (GXint32 j = 0; j < 4; j++) {
					view[i][j] = viewMat[k++];
				}
			}
			PVMatrix = projection * view; // Keep PVMatrix Updated
		}
		virtual inline GXMat4 getProjectionMatrix()const {
			return projection;
		}
		virtual void update()override;
		bool isInFrustum(const GXVec3& position, const GXuVec2& size) const {
			//TODO FRUSTUM CULLING ALGORITHM
			return true;
		}
	private:
		GXMat4 PVMatrix = GXMat4(1.0f);
		GXMat4 view = GXMat4(1.0f);
		GXMat4 projection = GXMat4(1.0f);
		virtual void updateCameraVectors();
		
		EditorCamera() :zNear(zNEAR),zFar(zFAR),AR(16.0f/9.0f){

		}
	};

}