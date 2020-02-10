#include "pch.h"
#include "Plane.h"

namespace gx {
	//heights must be bigger in width & height than plane
	void GXPlane::init(const float* heights)
	{
	
		std::vector<Vertex3D> verts;
		verts.reserve(width * depth);
		int widthMinusOne = width - 1;
		int depthMinusOne = depth - 1;
		//Using topLeft to center Plane aroung origin
		int topLeftX = widthMinusOne / -2.0f;
		int topLeftZ = depthMinusOne / -2.0f;
		int totalIndicesSize = widthMinusOne * depthMinusOne * 6;
		indices = new uint32_t[totalIndicesSize];
		for (int z = 0; z < depth; z++) {
			float v = static_cast<float>(z) / depth;
			for (int x = 0; x < width; x++) {
				float u = static_cast<float>(x) / width;			
				verts.emplace_back(
					GXVec3(topLeftX + x, heights[z*width+x], topLeftZ + z)// POS
					, GXVec3(0.0f, 1.0f, 0.0f)//NORMAL
					, GXVec2(u, v));// TEXCOORDS
			}
		}
		int k = 0;
		for (int z = 0; z < depthMinusOne;z++)
		{
			for (int x = 0; x < widthMinusOne;x++)
			{
				indices[k] = z * width + x;
				indices[k + 1] = (z + 1) * width + x;
				indices[k + 2] = z * width + x + 1;
				indices[k + 3] = (z + 1) * width + x;
				indices[k + 4] = (z + 1) * width + x + 1;
				indices[k + 5] = z * width + x + 1;
				k += 6; // next quad
			}
		}
		std::shared_ptr<GLBufferManager> Buffer;
		Buffer.reset(new GLBufferManager());
		Buffer->initFull(&verts[0], sizeof(Vertex3D) * verts.size(), sizeof(Vertex3D));
		Buffer->uploadIndicesToBuffer(indices, totalIndicesSize * sizeof(uint32_t), totalIndicesSize);
		Buffer->setAttribPointer(0, 3, GL_FLOAT, offsetof(Vertex3D, position));
		Buffer->setAttribPointer(1, 3, GL_FLOAT, offsetof(Vertex3D, normal));
		Buffer->setAttribPointer(2, 2, GL_FLOAT, offsetof(Vertex3D, texCoords));
		Buffer->setAttribPointer(3, 3, GL_FLOAT, offsetof(Vertex3D, tangent));
		Buffer->setAttribPointer(4, 3, GL_FLOAT, offsetof(Vertex3D, bitangent));
		//TODO Add Tangents bitangents later
		Buffer->endStream();
		/*for (int i = 0; i < textures.size(); i++) {
			Buffer->addTexture(textures[i]);
		}*/
		std::shared_ptr<GXMeshComponent> planeMeshComp;
		planeMeshComp.reset(new GXMeshComponent(Buffer));
		components.emplace_back(planeMeshComp);
	}

	void GXPlane::update(float deltaTime)
	{
		//For rendering as rendering happens in GXMeshComponent
#ifdef USING_OPENGL
		this->glshader->use();
		SceneLightManager::getInstance().setLightValues(this->glshader);
		this->glshader->setMat4("model", transform.getModel());
		this->glshader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
		this->glshader->setFloat("material.shininess", 32.0f);
#endif
		for (auto& component : components) {
			component->update(deltaTime);
			component->draw(glshader);
		}
	}

	void GXPlane::destroy()
	{
		for (int i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}	
	}
		
		
		
}