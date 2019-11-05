#include "pch.h"
#include "GLVertexBuffer.h"


namespace gx {
	void GLVertexBuffer::init(uint32_t requiredSize)
	{
		currentSize = 0;
		maxSize = requiredSize;
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, requiredSize, 0, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBuffer::uploadData(void* data, uint32_t size)
	{
		glBufferSubData(GL_ARRAY_BUFFER,currentSize,size,data);
		currentSize += size;
		GX_ASSERT(currentSize <= maxSize, "Vertex Buffer Size Violation !");
	}

	void GLVertexBuffer::destroy()
	{
		glDeleteBuffers(1, &ID);
	}
	
}