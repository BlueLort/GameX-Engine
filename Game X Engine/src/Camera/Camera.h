#pragma once
#include "Config.h"
#include "Maths/GXTransform.h"
#include "GX/GXTimer.h"
#include "Input/InputManager.h"

namespace gx {
	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	class GX_DLL Camera
	{
	public:
		Camera() : yaw(0.0f), pitch(0.0f), movementSpeed(30.0f), mouseSensitivity(5.0f), zoom(45.0f), cameraMoved(false){}
		GXTransform transform;
		// Euler Angles
		GXFloat yaw;
		GXFloat pitch;
		// Camera options
		GXFloat movementSpeed;
		GXFloat mouseSensitivity;
		GXFloat zoom;
		bool cameraMoved; // FOR LOD CONTROL
		virtual inline GXMat4 calculateViewMatrix()
		{
			return GXMaths::lookAt(transform.position, transform.position + transform.forward, transform.up);
		}
		virtual void processKeyboard(CameraMovement direction,float deltaTime) = 0;
		virtual void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;
		virtual void processMouseScroll(float yoffset) = 0;
		virtual void update() = 0;
	protected:
		
		virtual void updateCameraVectors()
		{
			GXVec3 front;
			front.x = cos(GXRad(yaw)) * cos(GXRad(pitch));
			front.y = sin(GXRad(pitch));
			front.z = sin(GXRad(yaw)) * cos(GXRad(pitch));
			transform.forward = GXMaths::normalize(front);

			transform.right = GXMaths::normalize(GXMaths::cross(transform.forward, transform.worldUp));
			transform.up = GXMaths::normalize(GXMaths::cross(transform.right, transform.forward));
		}
	};

}