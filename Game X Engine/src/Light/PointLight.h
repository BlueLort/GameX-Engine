#pragma once
#include "Light.h"

namespace gx {
	class GX_DLL PointLight : public Light {
	public:
		PointLight(GXVec3 Position, GXVec3 Ambient = GXVec3(0.1f, 0.1f, 0.1f)
			, GXVec3 Diffuse = GXVec3(0.8f, 0.8f, 0.8f)
			, GXVec3 Specular = GXVec3(1.0f, 1.0f, 1.0f)
			, float Constant = 1.0f
			, float Linear = 0.09f
			, float Quadratic = 0.032f
		) :
		position(Position),Light(Ambient,Diffuse,Specular)
		,constant(Constant),linear(Linear),quadratic(Quadratic)
		{

		}
		GXVec3 position;

		float constant;
		float linear;
		float quadratic;

	private:


	};
}