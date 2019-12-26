#include "pch.h"
#include "GLBufferManager.h"
namespace gx {


	void GLBufferManager::init(uint32_t requiredSize, uint32_t strideLength)
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

	void GLBufferManager::initFull(void* data, uint32_t requiredSize, uint32_t strideLength)
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

	void GLBufferManager::uploadDataToBuffer(void* data, uint32_t size)
	{
		VBO->uploadData(data, size);
	}


	void GLBufferManager::setAttribPointer(uint32_t loc, uint32_t count,GLenum type, uint32_t offset)
	{
		VAO->setAttribPointers(loc, count, type, offset);
	}

	void GLBufferManager::uploadIndicesToBuffer(uint32_t* indices, uint32_t size,uint32_t length)
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