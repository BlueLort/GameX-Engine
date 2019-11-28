#pragma once
#include "Light.h"

namespace gx {
	class GX_DLL SpotLight : public Light {
	public:
		SpotLight(GXVec3 Position,GXVec3 Direction, GXVec3 Ambient = GXVec3(0.1f, 0.1f, 0.1f)
			, GXVec3 Diffuse = GXVec3(0.8f, 0.8f, 0.8f)
			, GXVec3 Specular = GXVec3(1.0f, 1.0f, 1.0f)
			, float Constant = 1.0f
			, float Linear = 0.09f
			, float Quadratic = 0.032f
			, float cutOff=GXCos(GXRad(12.5f))
			, float outerCutOff =GXCos(GXRad(15.0f))
		) :
			position(Position),Light(Ambient,Diffuse,Specular)
			,constant(Constant), linear(Linear), quadratic(Quadratic)
			,cutOff(cutOff),outerCutOff(outerCutOff)
		{

		}
		GXVec3 position;
		GXVec3 direction;
		float constant;
		float linear;
		float quadratic;
		float cutOff;
		float outerCutOff;

	private:


	};
}