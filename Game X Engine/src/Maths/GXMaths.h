#pragma once
#include "Config.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

namespace gx {

#define CLAMP(a,start,end) if (a < start)a = start;else if (a > end)a = end;
#ifdef USING_GLM_MATHS
	using GXMat4 = glm::mat4;
	using GXMat3 = glm::mat3;
	using GXMat2 = glm::mat2;
	using GXVec4 = glm::vec4;
	using GXVec3 = glm::vec3;
	using GXVec2 = glm::vec2;
#define GXRad glm::radians
#define GXCos glm::cos
#define GXSin glm::sin
#define GXPI (3.14159265359)
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
	public:
		inline static double inverseLerp(double val,double a,double b){ return (val - a) / (b - a); }
		inline static double lerp(double f, double a, double b) { return a + f * (b - a); }
		inline static double cosinterp(double f,double a,double b){
			double newFactor = (1 - cos(f *GXPI)) / 2;
			return(a + newFactor * (b - a));
		}
		
		inline static GXMat4 prespective(const float& FOV,const float& AR,const float& zNear,const float& zFar ) {
			return glm::perspective(FOV,AR,zNear,zFar);
		}
		inline static GXMat4 lookAt(const GXVec3& pos, const GXVec3& target, const GXVec3& up) {
			return glm::lookAt(pos, target, up);
		}
		inline static GXMat4 translate(const GXMat4& Model,const GXVec3& Amount) {
			return glm::translate(Model,Amount);
		}
		inline static GXMat4 rotate(const GXMat4& Model,const float& Angle, const GXVec3& Axis) {
			return glm::rotate(Model,Angle,Axis);
		}
		inline static GXMat4 scale(const GXMat4& Model,const GXVec3& Amount) {
			return glm::scale(Model,Amount);
		}
		inline static GXMat4 inverse(const GXMat4& mat) {
			return glm::inverse(mat);
		}
		inline static GXMat3 inverse(const GXMat3& mat) {
			return glm::inverse(mat);
		}
		inline static GXVec3 normalize(const GXVec3& vec) {
			return glm::normalize(vec);
		}
		inline static GXVec3 cross(const GXVec3& vec1, const GXVec3& vec2) {
			return glm::cross(vec1, vec2);
		}

	};
}