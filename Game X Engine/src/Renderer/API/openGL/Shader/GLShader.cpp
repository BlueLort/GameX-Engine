#include "pch.h"
#include "GLShader.h"


namespace gx {
	GXuint32 GLShader::usedShaderID = 0;

	GLShader::GLShader()
	{
		// shader Program
		ID = glCreateProgram();

	}
	void GLShader::addShader(GXShaderType shaderType, const char* shaderCode)
	{
		if (shaderCode == nullptr)return;
		GXuint32 shaderID;
		char infoLog[1024];
		// Compile vertex and fragment shaders
		shaderID = compileShader(shaderType, shaderCode, infoLog);
		glAttachShader(ID, shaderID);
		linkProgram(infoLog);
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(shaderID);
	}
	GXuint32 GLShader::compileShader(GXShaderType type, const char* code, char* log)
	{

		GXuint32 shader = glCreateShader(type);
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);
		GXint32 success;
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
		GXint32 success;
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 1024, NULL, log);
			GXE_FERROR("FATAL PROGRAM LINKING ERROR\n{0}", log);
			GXE_ASSERT(false, "Linking Failed !");
		}
	}
}
