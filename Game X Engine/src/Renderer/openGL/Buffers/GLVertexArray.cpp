#include "pch.h"
#include "GLVertexArray.h"


namespace gx {
	void GLVertexArray::init(uint32_t StrideLength)
	{
		stride = StrideLength;
		GL_CALL(glGenVertexArrays(1, &ID));
	}

	void GLVertexArray::setAttribPointers(uint32_t ShaderLoc,uint32_t Count,GLenum Type,uint32_t offset)
	{

		GL_CALL(glVertexAttribPointer(ShaderLoc,Count,Type,GL_FALSE,stride,reinterpret_cast<void*>(offset)));
		GL_CALL(glEnableVertexAttribArray(ShaderLoc));
	}



}