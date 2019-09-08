#pragma once

#include <pch.h>
#include <Config.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
namespace gx {
#ifdef USING_GLM_MATHS
	using GXMat4 = glm::mat4;
	using GXMat3 = glm::mat3;
	using GXVec4 = glm::vec4;
	using GXVec3 = glm::vec3;
	#define GXRad glm::radians;
#else
	/* IF USING OTHER MATHS LIBRARIES*/
	//using GXMat4;
	//using GXMat3;
	//using GXVec4;
	//using GXVec3;
#endif // USING_GLM_MATHS
	//Static class that wraps the Maths Library used around the Engine
	//Adding whatever function i need to the wrapper before using it in the engine.
	class GX_DLL GXMaths {
		GXMaths() = delete;

		inline static GXMat4 Prespective(const float& FOV,const float& AR,const float& zNear,const float& zFar ) {
			return glm::perspective(FOV,AR,zNear,zFar);
		}
		inline static GXMat4 Translate(const GXMat4& Model,const GXVec3& Amount) {
			return glm::translate(Model,Amount);
		}
		inline static GXMat4 Rotate(const GXMat4& Model,const float& Angle, const GXVec3& Axis) {
			return glm::rotate(Model,Angle,Axis);
		}
		inline static GXMat4 Scale(const GXMat4& Model,const GXVec3& Amount) {
			return glm::scale(Model,Amount);
		}

	};
}