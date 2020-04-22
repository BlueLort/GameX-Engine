#pragma once
#include "Config.h"
#ifdef USING_OPENGL
 #include "Renderer/API/openGL/Shader/GLShader.h"
#endif 
#include "Maths/GXMaths.h"
#include "Renderer/Enums.h"
namespace gx {
#ifdef USING_OPENGL
	class GX_DLL GXShader
	{
	public:
		inline GXShader() {	}
		inline void use() {
			glShader.use();
		}
		inline static void stop() {
			GLShader::stop();
		}
		inline void setBool(const char* name, GXBool value) const {
			glShader.setBool(name,value);
		}
		inline void setInt(const char* name, GXint32 value) const {
			glShader.setInt(name,value);
		}

		inline void setUInt(const char* name, GXuint32 value) const {
			glShader.setUInt(name,value);
		}
		inline void setFloat(const  char* name, GXFloat value) const {
			glShader.setFloat(name, value);
		}
		inline void setVec2(const  char* name, const GXVec2& value) const {
			glShader.setVec2(name,value);
		}
		inline void setVec2(const char* name, GXFloat x, GXFloat y) const {
			glShader.setVec2(name, x, y);
		}
		inline void setVec3(const  char* name, const GXVec3& value) const {
			glShader.setVec3(name,value);
		}
		inline void setVec3(const char* name, GXFloat x, GXFloat y, GXFloat z) const {
			glShader.setVec3(name, x, y, z);
		}
		inline void setVec4(const  char* name, const GXVec4& value) const {
			glShader.setVec4(name,value);
		}
		inline void setVec4(const  char* name, GXFloat x, GXFloat y, GXFloat z, GXFloat w) const {
			glShader.setVec4(name, x,y,z,w);
		}
		inline void setMat2(const char* name, const GXMat2& mat) const {
			glShader.setMat2(name, mat);
		}
		inline void setMat3(const char* name, const GXMat3& mat) const {
			glShader.setMat3(name, mat);
		}
		inline void setMat4(const char* name, const GXMat4& mat) const {
			glShader.setMat4(name, mat);
		}
		inline void update(GXMat4 proj,GXMat4 view,GXMat4 model,GXVec3 camPos) {
			glShader.update(proj, view, model, camPos);
		}
		void addShader(GXShaderType shaderType, const char* shaderCode) {
			glShader.addShader(shaderType, shaderCode);
		}
		~GXShader() { glShader.destroy(); }
	private:
		GLShader glShader;
	};
#endif
}