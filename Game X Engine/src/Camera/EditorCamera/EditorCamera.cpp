#include "pch.h"
#include "EditorCamera.h"

namespace gx {
	
	void EditorCamera::init(const GXVec3& position, const GXVec3& up, GXFloat yaw, GXFloat pitch,GXFloat AR,GXFloat zNear,GXFloat zFar)
	{
		transform.position = position;
		transform.worldUp = up;
		transform.forward = GXVec3(0.0f, 0.0f, -1.0f);
		movementSpeed = SPEED;
		mouseSensitivity = SENSITIVITY;
		zoom = ZOOM;
		this->yaw = yaw;
		this->pitch = pitch;
		this->AR = AR;
		this->zNear = zNear;
		this->zFar = zFar;
		projection= GXMaths::prespective(zoom, AR, zNEAR, zFAR);
		updateCameraVectors();

	}
	//Controlled by input manager
	void EditorCamera::processMouseScroll(GXFloat yoffset)
	{
			if (zoom >= 1.0f && zoom <= 45.0f)
				zoom -= yoffset;
			if (zoom <= 1.0f)
				zoom = 1.0f;
			if (zoom >= 45.0f)
				zoom = 45.0f;
			projection = GXMaths::prespective(GXRad(zoom), AR, zNEAR, zFAR);
			PVMatrix = projection * view;
	}



	void EditorCamera::updateCameraVectors()
	{
		GXVec3 front;
		front.x = cos(GXRad(yaw)) * cos(GXRad(pitch));
		front.y = sin(GXRad(pitch));
		front.z = sin(GXRad(yaw)) * cos(GXRad(pitch));
		transform.forward = GXMaths::normalize(front);

		transform.right = GXMaths::normalize(GXMaths::cross(transform.forward, transform.worldUp));
		transform.up = GXMaths::normalize(GXMaths::cross(transform.right, transform.forward));
		
		view = GXMaths::lookAt(transform.position, transform.position + transform.forward, transform.up);
		PVMatrix = projection * view;
	}

	void EditorCamera::processKeyboard(CameraMovement direction,GXFloat deltaTime)
	{
		
		float velocity = movementSpeed * deltaTime;
		if (direction == FORWARD)
			transform.position += transform.forward * velocity;
		else if (direction == BACKWARD)
			transform.position -= transform.forward * velocity;
		else if (direction == LEFT)
			transform.position -= transform.right * velocity;
		else if (direction == RIGHT)
			transform.position += transform.right * velocity;

		updateCameraVectors();
	}
	void EditorCamera::processMouseMovement(GXFloat xoffset, GXFloat yoffset, GXBool constrainPitch)
	{
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}
		updateCameraVectors();
	}
	void EditorCamera::update()
	{
		float deltaTime = 1.0f / GXTimer::getAppTimer().getDeltaTicks();
		if (InputManager::getInstance().isPressed(event::key::GXK_W))
			processKeyboard(CameraMovement::FORWARD,deltaTime);
		if (InputManager::getInstance().isPressed(event::key::GXK_S))
			processKeyboard(CameraMovement::BACKWARD,deltaTime);
		if (InputManager::getInstance().isPressed(event::key::GXK_A))
			processKeyboard(CameraMovement::LEFT,deltaTime);
		if (InputManager::getInstance().isPressed(event::key::GXK_D))
			processKeyboard(CameraMovement::RIGHT,deltaTime);

		processMouseScroll(InputManager::getInstance().getMouseWheel().second);

		float moveSpeed = MOVEMENT_SPEED*deltaTime;
		//Change mouse movement Handling later
		if (InputManager::getInstance().isPressed(event::key::GXK_UP))
			processMouseMovement(0.0f, moveSpeed);
		if (InputManager::getInstance().isPressed(event::key::GXK_DOWN))
			processMouseMovement(0.0f,-moveSpeed);
		if (InputManager::getInstance().isPressed(event::key::GXK_LEFT))
			processMouseMovement(-moveSpeed, 0.0f);
		if (InputManager::getInstance().isPressed(event::key::GXK_RIGHT))
			processMouseMovement(moveSpeed, 0.0f);

	}
}
