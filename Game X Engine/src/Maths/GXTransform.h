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
			GXMat4 rotMat(1.0f);
			//TODO MOVE TO Quaternions
			rotMat = GXMaths::rotate(rotMat, GXRad(rotation.x), GXVec3(1.0f, 0.0f, 0.0f)) * GXMaths::rotate(rotMat, GXRad(rotation.y), GXVec3(0.0f, 1.0f, 0.0f)) * GXMaths::rotate(rotMat, GXRad(rotation.z), GXVec3(0.0f, 0.0f, 1.0f));
			model = model * rotMat;
			model = GXMaths::scale(model, scale);
			return model;
		}

		inline GXMat4 getLookAt(const GXVec3& target) {

			return GXMat4();
		}

	};

}