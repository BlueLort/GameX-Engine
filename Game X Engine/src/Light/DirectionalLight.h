#pragma once
#include "Light.h"

namespace gx {
	class GX_DLL DirectionalLight : public Light {
	public:
		DirectionalLight(GXVec3 Direction, GXVec3 Ambient = GXVec3(0.1f, 0.1f, 0.1f)
			, GXVec3 Diffuse = GXVec3(0.8f, 0.8f, 0.8f)
			, GXVec3 Specular = GXVec3(1.0f, 1.0f, 1.0f)) :
			direction(Direction), Light(Ambient, Diffuse, Specular)
			,virtualPos(-Direction)
		{
			virtualPos *= 5.0f;//make position for the same direction
			lightProjection = GXMaths::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 1.0f, 70.5f);
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