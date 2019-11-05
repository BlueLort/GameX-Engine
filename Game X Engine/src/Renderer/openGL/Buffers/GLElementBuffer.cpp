#include "pch.h"
#include "GLElementBuffer.h"

namespace gx {
	void GLElementBuffer::init()
	{
		glGenBuffers(1, &ID);
	}

	void GLElementBuffer::uploadData(uint32_t* indices,uint32_t size)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	void GLElementBuffer::destroy()
	{
		glDeleteBuffers(1, &ID);
	}
}