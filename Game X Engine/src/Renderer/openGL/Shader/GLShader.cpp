#include "pch.h"
#include "GLShader.h"


namespace gx {


	GLShader::GLShader(const char* vertexShaderCode, const char* fragmentShaderCode, const char* geometryShaderCode)
	{
		bool geoExist = geometryShaderCode != nullptr;
		uint32_t vertex, fragment, geometry;
		char infoLog[1024];
		// Compile vertex and fragment shaders
		vertex = compileShader(GL_VERTEX_SHADER, vertexShaderCode, infoLog);
		fragment = compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode, infoLog);

		// if geometry shader is given, compile geometry shader
		if (geoExist)
		{
			geometry = compileShader(GL_GEOMETRY_SHADER, geometryShaderCode, infoLog);
		}
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geoExist)
			glAttachShader(ID, geometry);

		linkProgram(infoLog);
		
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geoExist)
			glDeleteShader(geometry);

	}
	GLuint GLShader::compileShader(GLuint type, const char* code, char* log)
	{

		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, log);
			GXE_FERROR("FATAL PROGRAM LINKING ERROR , Type: {1} \n{0}", log, type);
			GXE_ASSERT(false, "Compilation Failed !");
		}
		return shader;
	}
	void GLShader::linkProgram(char* log)
	{
		glLinkProgram(ID);
		int success;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 1024, NULL, log);
			GXE_FERROR("FATAL PROGRAM LINKING ERROR\n{0}", log);
			GXE_ASSERT(false, "Linking Failed !");
		}
	}
}
