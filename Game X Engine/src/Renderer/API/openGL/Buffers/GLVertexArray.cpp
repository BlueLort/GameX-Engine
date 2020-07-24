#include "pch.h"
#include "GLVertexArray.h"


namespace gx {
	void GLVertexArray::init(GXuint32 StrideLength)
	{
		stride = StrideLength;
		GL_CALL(glGenVertexArrays(1, &ID));
	}

	void GLVertexArray::setAttribPointer(GXuint32 ShaderLoc,GXuint32 Count,GLenum Type,GXuint32 offset)
	{

		GL_CALL(glVertexAttribPointer(ShaderLoc,Count,Type,GL_FALSE,stride,reinterpret_cast<void*>(offset)));
		GL_CALL(glEnableVertexAttribArray(ShaderLoc));
	}
	void GLVertexArray::setAttribIPointer(GXuint32 ShaderLoc, GXuint32 Count, GLenum Type, GXuint32 offset)
	{

		GL_CALL(glVertexAttribIPointer(ShaderLoc, Count, Type, stride, reinterpret_cast<void*>(offset)));
		GL_CALL(glEnableVertexAttribArray(ShaderLoc));
	}
	void GLVertexArray::setAttribPointer(GXuint32 ShaderLoc, GXuint32 Count, GLenum Type, GXuint32 offset,GXuint32 strideSize)
	{

		GL_CALL(glVertexAttribPointer(ShaderLoc, Count, Type, GL_FALSE, strideSize, reinterpret_cast<void*>(offset)));
		GL_CALL(glEnableVertexAttribArray(ShaderLoc));
	}
	void GLVertexArray::setAttribIPointer(GXuint32 ShaderLoc, GXuint32 Count, GLenum Type, GXuint32 offset, GXuint32 strideSize)
	{

		GL_CALL(glVertexAttribIPointer(ShaderLoc, Count, Type, strideSize, reinterpret_cast<void*>(offset)));
		GL_CALL(glEnableVertexAttribArray(ShaderLoc));
	}

	void GLVertexArray::setVertexAttribDivisor(GXuint32 loc, GXuint32 divisor)
	{
		GL_CALL(glVertexAttribDivisor(loc, divisor));
	}


}