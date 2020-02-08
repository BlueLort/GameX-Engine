#pragma once
#include "Maths/GXMaths.h"
namespace gx {
	struct Vertex3D {
		GXVec3 position;
		GXVec3 normal;
		GXVec3 tangent;
		GXVec3 bitangent;
		GXVec2 texCoords;
		Vertex3D() {

		}
		Vertex3D(GXVec3 Position,
		GXVec3 Normal,
		GXVec2 TexCoords,
		GXVec3 Tangent= GXVec3(0.0f, 0.0f, 0.0f),
		GXVec3 Bitangent=GXVec3(0.0f,0.0f,0.0f)):
			position(Position),
			normal(Normal),
			tangent(Tangent),
			bitangent(Bitangent),
			texCoords(TexCoords)
		{

		}
	};
}
