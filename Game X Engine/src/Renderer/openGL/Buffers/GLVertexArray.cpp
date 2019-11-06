#include "pch.h"
#include "GLVertexArray.h"


namespace gx {
	void GLVertexArray::init(uint32_t StrideLength)
	{
		stride = StrideLength;
		glGenVertexArrays(1, &ID);
	}

	void GLVertexArray::setAttribPointers(uint32_t ShaderLoc,uint32_t Count,GLenum Type,uint32_t offset)
	{

		glVertexAttribPointer(ShaderLoc,Count,Type,GL_FALSE,stride,reinterpret_cast<void*>(offset));
		glEnableVertexAttribArray(ShaderLoc);
	}



}