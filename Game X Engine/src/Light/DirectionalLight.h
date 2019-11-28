#pragma once
#include "Light.h"

namespace gx {
	class GX_DLL DirectionalLight : public Light {
	public:
		DirectionalLight(GXVec3 Direction, GXVec3 Ambient = GXVec3(0.1f, 0.1f, 0.1f)
			, GXVec3 Diffuse = GXVec3(0.8f, 0.8f, 0.8f)
			, GXVec3 Specular = GXVec3(1.0f, 1.0f, 1.0f)) :
			direction(Direction), Light(Ambient, Diffuse, Specular)
		{

		}
		GXVec3 direction;
	private:


	};
}