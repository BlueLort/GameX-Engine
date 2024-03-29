#include "pch.h"
#include "GLVertexBuffer.h"


namespace gx {
	void GLVertexBuffer::init(GXuint32 requiredSize)
	{
		currentSize = 0;
		maxSize = requiredSize;

		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, requiredSize, 0, GL_STATIC_DRAW));
		//Can't UNBIND THE VBO Before Registering it using VAO Attrib Pointer.
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLVertexBuffer::initFull(void* data, GXuint32 requiredSize)
	{
		currentSize = requiredSize;
		maxSize = requiredSize;
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, requiredSize, data, GL_STATIC_DRAW));
		//Can't UNBIND THE VBO Before Registering it using VAO Attrib Pointer.
		//GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void GLVertexBuffer::uploadData(void* data, GXuint32 size)
	{
		GL_CALL(glBufferSubData(GL_ARRAY_BUFFER,currentSize,size,data));
		currentSize += size;
		GX_ASSERT(currentSize <= maxSize, "Vertex Buffer Size Violation !");
	}
	void GLVertexBuffer::updateData(void* data, GXuint32 requiredSize)
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, ID));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, requiredSize, data, GL_DYNAMIC_DRAW));
	}


	
}