#include "pch.h"
#include "GLBufferManager.h"
namespace gx {
	std::map<std::string, GLuint> GLBufferManager::storedData;

	void GLBufferManager::init(const std::string& ID, uint32_t requiredSize, uint32_t strideLength)
	{
		auto ite = storedData.find(ID);
		if (ite!=storedData.end()) {
			VAOID = ite->second;
		}
		else {
			VAO.reset(new GLVertexArray());
			VBO.reset(new GLVertexBuffer());
			EBO.reset(new GLElementBuffer());

			VAOID = VAO->ID;
			storedData[ID] = VAOID;

			VAO->init(strideLength);
			VBO->init(requiredSize);
			EBO->init();
		}
	}

	void GLBufferManager::init(uint32_t requiredSize, uint32_t strideLength)
	{//NO ID SAVING
		VAO.reset(new GLVertexArray());
		VBO.reset(new GLVertexBuffer());
		EBO.reset(new GLElementBuffer());

		VAOID = VAO->ID;

		VAO->init(strideLength);
		VBO->init(requiredSize);
		EBO->init();
	}

	void GLBufferManager::initFull(const std::string& ID, void* data, uint32_t requiredSize, uint32_t strideLength)
	{
		auto ite = storedData.find(ID);
		if (ite != storedData.end()) {
			VAOID = ite->second;
		}
		else {
			VAO.reset(new GLVertexArray());
			VBO.reset(new GLVertexBuffer());
			EBO.reset(new GLElementBuffer());

			VAOID = VAO->ID;
			storedData[ID] = VAOID;

			VAO->init(strideLength);
			VBO->initFull(data,requiredSize);
			EBO->init();
		}
	}

	void GLBufferManager::initFull(void* data, uint32_t requiredSize, uint32_t strideLength)
	{
		//NO ID SAVING
		VAO.reset(new GLVertexArray());
		VBO.reset(new GLVertexBuffer());
		EBO.reset(new GLElementBuffer());

		VAOID = VAO->ID;

		VAO->init(strideLength);
		VBO->initFull(data,requiredSize);
		EBO->init();
	}

	void GLBufferManager::uploadDataToBuffer(void* data, uint32_t size)
	{
		GLVertexArray::use(VAOID);
		VBO->use();
		VBO->uploadData(data, size);
		GLVertexBuffer::stop();
		GLVertexArray::stop();
	}


	void GLBufferManager::setAttribPointer(uint32_t loc, uint32_t count,GLenum type, uint32_t offset)
	{
		GLVertexArray::use(VAOID);
		VAO->setAttribPointers(loc, count, type, offset);
		GLVertexArray::stop();
	}

	void GLBufferManager::uploadIndicesToBuffer(uint32_t* indices, uint32_t size)
	{
		GLVertexArray::use(VAOID);
		EBO->use();
		EBO->uploadData(indices, size);
		GLElementBuffer::stop();
		GLVertexArray::stop();
	}

	void GLBufferManager::endStream()
	{
		VBO->destroy();
		EBO->destroy();
		delete VAO.release();
		delete VBO.release();
		delete EBO.release();
	}

	void GLBufferManager::destroy()//Delete all Vertex Arrays 
	{
		for (auto ite : storedData) {
			GLVertexArray::destroy(ite.second);
		}
			
	}

}