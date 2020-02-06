#pragma once

#include "pch.h"
#include "Config.h"
#include "Window/GXWindow.h"
#include "Maths/GXMaths.h"

namespace gx {
	enum GXShaderType {
#ifdef USING_OPENGL
		GX_VERTEX_SHADER=GL_VERTEX_SHADER,
		GX_FRAGMENT_SHADER=GL_FRAGMENT_SHADER,
		GX_COMPUTE_SHADER=GL_COMPUTE_SHADER,
		GX_GEOMETRY_SHADER=GL_GEOMETRY_SHADER,
		GX_TESS_CONTROL_SHADER=GL_TESS_CONTROL_SHADER,
		GX_TESS_EVALUATION_SHADER=GL_TESS_EVALUATION_SHADER
#endif
	};
	class GX_DLL GLShader
	{
	public:
		
		// activate the shader
		// ------------------------------------------------------------------------
		inline void use() {
			glUseProgram(ID);
		}
		// deactivate shaders
		// ------------------------------------------------------------------------
		inline static void stop() {
			glUseProgram(0);
		}
		// utility uniform functions
		// ------------------------------------------------------------------------
		inline void setBool(const char* name, bool value) const {
			GLuint loc = glGetUniformLocation(ID, name);
			glUniform1i(loc, static_cast<int>(value));
		}
		// ------------------------------------------------------------------------
		inline void setInt(const char* name, int value) const{
			GLuint loc = glGetUniformLocation(ID, name);
			glUniform1i(loc, value);
		}

		// ------------------------------------------------------------------------
		inline void setFloat(const  char* name, float value) const {
			GLuint loc = glGetUniformLocation(ID, name);
			glUniform1f(loc, value);
		}
		// ------------------------------------------------------------------------
		inline void setVec2(const  char* name, const GXVec2& value) const {
			GLuint loc = glGetUniformLocation(ID, name);
			glUniform2fv(loc, 1, &value[0]);
		}
		inline void setVec2(const char* name, float x, float y) const {
			GLuint loc = glGetUniformLocation(ID, name);
			glUniform2f(loc, x, y);
		}
		// ------------------------------------------------------------------------
		inline void setVec3(const  char* name, const GXVec3& value) const {
			GLuint loc = glGetUniformLocation(ID, name); 
			glUniform3fv(loc, 1, &value[0]);
		}
		inline void setVec3(const char* name, float x, float y, float z) const {
			GLuint loc = glGetUniformLocation(ID, name); 
			glUniform3f(loc, x, y, z);
		}
		// ------------------------------------------------------------------------
		inline void setVec4(const  char* name, const GXVec4& value) const { 
			GLuint loc = glGetUniformLocation(ID, name); 
			glUniform4fv(loc, 1, &value[0]);
		}
		inline void setVec4(const  char* name, float x, float y, float z, float w) const {
			GLuint loc = glGetUniformLocation(ID, name);
			glUniform4f(loc, x, y, z, w);
		}
		// ------------------------------------------------------------------------
		inline void setMat2(const char* name, const GXMat2& mat) const {
			GLuint loc = glGetUniformLocation(ID, name); 
			glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		inline void setMat3(const char* name, const GXMat3& mat) const { 
			GLuint loc = glGetUniformLocation(ID, name); 
			glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
		// ------------------------------------------------------------------------
		inline void setMat4(const char* name, const GXMat4& mat) const {
			GLuint loc = glGetUniformLocation(ID, name);
			glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
		}
		//---------------------------------------------------------------------
		//updates the shader with known Shader Values Such as projection view light pos.
		inline void update(glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::vec3 camPos) {

			//update shader data

		}
		//---------------------------------------------------------------------

		~GLShader() { glDeleteProgram(ID); }
	private:
		GLShader();
		void addShader(GXShaderType shaderType,const char* shaderCode);
		GLuint ID;
		GLuint compileShader(GXShaderType type, const char* code, char* log);
		void linkProgram(char* log);
		friend class GLShaderManager;
	};
}