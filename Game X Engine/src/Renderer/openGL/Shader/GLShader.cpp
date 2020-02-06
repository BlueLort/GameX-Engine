#include "pch.h"
#include "GLShader.h"


namespace gx {


	GLShader::GLShader()
	{
		// shader Program
		ID = glCreateProgram();

	}
	void GLShader::addShader(GXShaderType shaderType, const char* shaderCode)
	{
		if (shaderCode == nullptr)return;
		uint32_t shaderID;
		char infoLog[1024];
		// Compile vertex and fragment shaders
		shaderID = compileShader(shaderType, shaderCode, infoLog);
		glAttachShader(ID, shaderID);
		linkProgram(infoLog);
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(shaderID);
	}
	GLuint GLShader::compileShader(GXShaderType type, const char* code, char* log)
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
