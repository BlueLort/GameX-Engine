#include "pch.h"
#include "GLBufferManager.h"
namespace gx {


	void GLBufferManager::init(GXuint32 requiredSize, GXuint32 strideLength)
	{//NO ID SAVING
		VAO.reset(new GLVertexArray());
		VBO.reset(new GLVertexBuffer());
		EBO.reset(new GLElementBuffer());

		

		VAO->init(strideLength);
		VAOID = VAO->ID;
		GLVertexArray::use(VAOID);
		VBO->init(requiredSize);
		GLVertexArray::stop();
		EBO->init();
	}

	void GLBufferManager::initFull(void* data, GXuint32 requiredSize, GXuint32 strideLength)
	{
		//NO ID SAVING
		VAO.reset(new GLVertexArray());
		VBO.reset(new GLVertexBuffer());
		EBO.reset(new GLElementBuffer());

		VAO->init(strideLength);
		VAOID = VAO->ID;
		GLVertexArray::use(VAOID);
		VBO->initFull(data,requiredSize);
		VBOID = VBO->ID;
		EBO->init();
		EBOID = EBO->ID;
	}

	void GLBufferManager::uploadDataToBuffer(void* data, GXuint32 size)
	{
		VBO->uploadData(data, size);
	}


	void GLBufferManager::setAttribPointer(GXuint32 loc, GXuint32 count,GLenum type, GXuint32 offset)
	{
		VAO->setAttribPointers(loc, count, type, offset);
	}

	void GLBufferManager::uploadIndicesToBuffer(GXuint32* indices, GXuint32 size,GXuint32 length)
	{
		nElements = length;
		EBO->use();
		EBO->uploadData(indices, size);
		// Can't unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO.
		//GLElementBuffer::stop();
	}

	void GLBufferManager::endStream()
	{
		

		//CAN'T UNBIND THE EBO BEFORE THE GLVERTEX ARRAY.
		GLVertexArray::stop();
		GLElementBuffer::stop();
		GLVertexBuffer::stop();
		delete VAO.release();
		delete VBO.release();
		delete EBO.release();
	}

}