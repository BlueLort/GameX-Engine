#pragma once
#include "Light.h"

namespace gx {
	class GX_DLL DirectionalLight : public Light {
	public:
		DirectionalLight(GXVec3 Direction, GXVec3 Ambient = GXVec3(0.2f, 0.2f, 0.2f)
			, GXVec3 Diffuse = GXVec3(0.9f, 0.9f, 0.9f)
			, GXVec3 Specular = GXVec3(1.0f, 1.0f, 1.0f)) :
			direction(Direction), Light(Ambient, Diffuse, Specular)
			,virtualPos(Direction)
		{
			virtualPos *= 2.0f;//make position for the same direction
			virtualPos.y *= -3.0f;
			lightProjection = GXMaths::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 128.5f);
			lightView = GXMaths::lookAt(virtualPos, GXVec3(0.0f, 0.0f, 0.0f), GXVec3(0.0f, 1.0f, 0.0f));
			lightSpaceMatrix = lightProjection* lightView;
		}
		GXVec3 direction;
		GXVec3 virtualPos;
		GXMat4 lightProjection;
		GXMat4 lightView;
		GXMat4 lightSpaceMatrix;

	private:


	};
}