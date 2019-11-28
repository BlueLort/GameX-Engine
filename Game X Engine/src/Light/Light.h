#pragma once
#include "Config.h"
#include "Maths/GXMaths.h"

namespace gx {
	class GX_DLL Light {
	public:
		Light(GXVec3 Ambient, GXVec3 Diffuse, GXVec3 Specular):ambientColor(Ambient), diffuseColor(Diffuse), specularColor(Specular) {

		}
		GXVec3 ambientColor;
		GXVec3 diffuseColor;
		GXVec3 specularColor;

	};
}