#pragma once
#include "Config.h"
#include "GXMaths.h"
namespace gx {

	class GX_DLL GXTransform {
	public:
		
		GXTransform() :
		position(0.0f, 0.0f, 0.0f),
		scale(1.0f, 1.0f, 1.0f),
		rotation(0.0f, 0.0f, 0.0f),
		up(0.0f, 1.0f, 0.0f),
		worldUp(0.0f,1.0f,0.0f),
		forward(0.0f,0.0f,1.0f),
		right(1.0f,0.0f,0.0f)
		{
		}
		GXVec3 position;
		GXVec3 scale;
		//angles in degrees
		GXVec3 rotation;
		GXVec3 up;
		GXVec3 worldUp;
		GXVec3 forward;
		GXVec3 right;


		inline GXMat4 getModel() {
			GXMat4 model(1.0f);
			model = GXMaths::translate(model, position);
			model = GXMaths::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			model = GXMaths::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			model = GXMaths::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			model = GXMaths::scale(model, scale);
			return model;
		}

		inline GXMat4 getLookAt(const GXVec3& target) {

			return GXMat4();
		}

	};

}