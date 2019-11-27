#include "pch.h"
#include "GLElementBuffer.h"

namespace gx {
	void GLElementBuffer::init()
	{
		GL_CALL(glGenBuffers(1, &ID));
	}

	void GLElementBuffer::uploadData(uint32_t* indices,uint32_t size)
	{
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW));
	}
}