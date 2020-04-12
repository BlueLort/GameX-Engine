#include "pch.h"
#include "GLVertexArray.h"


namespace gx {
	void GLVertexArray::init(GXuint32 StrideLength)
	{
		stride = StrideLength;
		GL_CALL(glGenVertexArrays(1, &ID));
	}

	void GLVertexArray::setAttribPointers(GXuint32 ShaderLoc,GXuint32 Count,GLenum Type,GXuint32 offset)
	{

		GL_CALL(glVertexAttribPointer(ShaderLoc,Count,Type,GL_FALSE,stride,reinterpret_cast<void*>(offset)));
		GL_CALL(glEnableVertexAttribArray(ShaderLoc));
	}



}