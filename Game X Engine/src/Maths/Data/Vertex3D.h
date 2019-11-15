#pragma once
#include "../GXMaths.h"
namespace gx {
	struct Vertex3D {
		GXVec3 position;
		GXVec3 normal;
		GXVec3 tangent;
		GXVec3 bitangent;
		GXVec2 texCoords;

	};
}
