#include "pch.h"
#include "GLBufferManager.h"
namespace gx {


	void GLBufferManager::init(GXuint32 requiredSize, GXuint32 strideLength)
	{//NO ID SAVING
		VAO.reset(new GLVertexArray());
		staticVBO.reset(new GLVertexBuffer());
		dynamicVBO.reset(new GLVertexBuffer());
		EBO.reset(new GLElementBuffer());

		VAO->init(strideLength);
		VAOID = VAO->ID;
		GLVertexArray::use(VAOID);
		staticVBO->init(requiredSize);
		GLVertexArray::stop();
		EBO->init();
	}
	
	void GLBufferManager::initFull(void* data, GXuint32 requiredSize, GXuint32 strideLength)
	{
		//NO ID SAVING
		VAO.reset(new GLVertexArray());
		staticVBO.reset(new GLVertexBuffer());
		dynamicVBO.reset(new GLVertexBuffer());
		EBO.reset(new GLElementBuffer());

		VAO->init(strideLength);
		VAOID = VAO->ID;
		GLVertexArray::use(VAOID);
		staticVBO->initFull(data,requiredSize);
		sVBOID = staticVBO->ID;
		dVBOID = dynamicVBO->ID;
		EBO->init();
		EBOID = EBO->ID;
	}

	void GLBufferManager::uploadDataToBuffer(void* data, GXuint32 size)
	{
		staticVBO->uploadData(data, size);
	}
	void GLBufferManager::updateDynamicBuffer(void* data, GXuint32 size)
	{
		dynamicVBO->updateData(data, size);
	}

	void GLBufferManager::setAttribPointer(GXuint32 loc, GXuint32 count,GXEnumType type, GXuint32 offset)
	{
		VAO->setAttribPointer(loc, count, type, offset);
	}
	void GLBufferManager::setAttribIPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset) {
		VAO->setAttribIPointer(loc, count, type, offset);
	}
	void GLBufferManager::setAttribPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset, GXuint32 stride)
	{
		VAO->setAttribPointer(loc, count, type, offset, stride);
	}
	void GLBufferManager::setAttribIPointer(GXuint32 loc, GXuint32 count, GXEnumType type, GXuint32 offset, GXuint32 stride) {
		VAO->setAttribIPointer(loc, count, type, offset, stride);
	}
	void GLBufferManager::setVertexAttribDivisor(GXuint32 loc, GXuint32 divisor)
	{
		VAO->setVertexAttribDivisor(loc, divisor);
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
		delete staticVBO.release();
		delete EBO.release();
	}

}