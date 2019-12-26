#pragma once
#include "Config.h"
#include "pch.h"
#include "../Camera.h"
//TODO ADD CONTROL LOGIC
namespace gx {
	/*Most logic copied from learnopengl.com ...just a change in the convention.*/

	// Default camera values
	constexpr float YAW = -90.0f;
	constexpr float PITCH = 0.0f;
	constexpr float SPEED = 1.0f;
	constexpr float SENSITIVITY = 1.25f;
	constexpr float ZOOM = 45.0f;
	constexpr float MOVEMENT_SPEED = 16.0f;
	// Default Projection values
	constexpr float ASPECT_RATIO = 1024.0f / 768.0f;
	constexpr float zNEAR = 0.1f;
	constexpr float zFAR = 1000.0f;
	class GX_DLL EditorCamera:public Camera
	{
	public:
		float zNear, zFar, AR;
		static EditorCamera& getInstance(){ 
			static EditorCamera ecam;
			return ecam;
		}
		void init(const GXVec3& position = GXVec3(0.0f, 0.0f, 10.0f), const GXVec3& up = GXVec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float AR=ASPECT_RATIO, float zNear=zNEAR, float zFar=zFAR);

		virtual void processKeyboard(CameraMovement direction,float deltaTime)override;
		virtual void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;
		virtual void processMouseScroll(float yoffset) override;
		virtual inline GXMat4 getPVMatrix() const { return PVMatrix; }
		virtual inline GXMat4 getViewMatrix() const
		{
			return view;
		}
		virtual inline GXMat4 getProjectionMatrix()const {
			return projection;
		}
		virtual void update()override;
	private:
		GXMat4 PVMatrix;
		GXMat4 view;
		GXMat4 projection;
		virtual void updateCameraVectors();
		
		EditorCamera() {

		}
	};

}