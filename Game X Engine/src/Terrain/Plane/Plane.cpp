#include "pch.h"
#include "Plane.h"

namespace gx {
	//heights must be bigger in width & height than plane
	void GXPlane::init()
	{
		GXFloat dx = static_cast<float>(width) / clustringFactor.x;
		GXFloat dz = static_cast<float>(depth) / clustringFactor.y;

		//Using topLeft to center Plane aroung origin
		GXFloat topLeftX = (width) / -2.0f;
		GXFloat topLeftZ = (depth) / -2.0f;

		totalIndicesSize = clustringFactor.x * clustringFactor.y * 6;
		indices = new uint32_t[totalIndicesSize];
		GXFloat posZ = topLeftZ;
		for (GXint32 i = 0; i <= clustringFactor.y; i++)
		{
			GXFloat posX = topLeftX;
			for (GXint32 j = 0; j <= clustringFactor.x; j++)
			{
				verts.emplace_back(
					GXVec2(posX, posZ)// POS
					, GXiVec2(j, i));// Sampling Offset
				posX += dx;
			}
			posZ += dz;
		}
		GXint32 k = 0;
		//we have actually n+1 * m+1 vertices to get -width/2 to exactly width/2 and -depth/2 to depth/2
		GXint32 xClusterPlusOne = clustringFactor.x + 1;
		for (GXint32 z = 0; z < clustringFactor.y; z++)
		{
			for (GXint32 x = 0; x < clustringFactor.x; x++)
			{
				indices[k] = z * xClusterPlusOne + x;
				indices[k + 1] = (z + 1) * xClusterPlusOne + x;
				indices[k + 2] = z * xClusterPlusOne + x + 1;

				indices[k + 3] = (z + 1) * xClusterPlusOne + x;
				indices[k + 4] = (z + 1) * xClusterPlusOne + x + 1;
				indices[k + 5] = z * xClusterPlusOne + x + 1;
				k += 6; // next quad
			}
		}

		//Per instance data [ all starting LOD 0 but different postions and texture locations
		for (int32_t i = 0; i < rowInstances; i++)
		{
			for (int32_t j = 0; j < colInstances; j++)
			{
				instances.emplace_back(
					glm::vec2(j * width - centerOffset.x, i * depth - centerOffset.y)// POS offset
					, glm::ivec2(j * clustringFactor.x, i * clustringFactor.y) // Sampling Offset
					, 0); // LOD
			}
		}

	}

	void GXPlane::uploadToBuffer(GXuint32 difftextureID, GXuint32 hmaptextureID)
	{
		
		Buffer.reset(new GXGraphicsBufferManager());
		Buffer->initFull(&verts[0], sizeof(PlaneVertex3D) * verts.size(), sizeof(PlaneVertex3D));
		Buffer->uploadIndicesToBuffer(&indices[0], totalIndicesSize * sizeof(GXuint32), totalIndicesSize);
		Buffer->setAttribPointer(0, 2, GX_FLOAT, offsetof(PlaneVertex3D, position), sizeof(PlaneVertex3D));
		Buffer->setAttribIPointer(1, 2, GX_INT, offsetof(PlaneVertex3D, samplingOffset), sizeof(PlaneVertex3D));
		Buffer->bindDynamicBuffer();
		Buffer->setAttribPointer(2, 2, GX_FLOAT, offsetof(PlaneInstanceData, positionOffset), sizeof(PlaneInstanceData));
		Buffer->setAttribIPointer(3, 2, GX_INT, offsetof(PlaneInstanceData, samplingStartOffset), sizeof(PlaneInstanceData));
		Buffer->setAttribIPointer(4, 1, GX_INT, offsetof(PlaneInstanceData, levelLOD), sizeof(PlaneInstanceData));
		Buffer->setVertexAttribDivisor(2, 1);
		Buffer->setVertexAttribDivisor(3, 1);
		Buffer->setVertexAttribDivisor(4, 1);
		Buffer->endStream();
		std::shared_ptr<GXTexture2D> tex;
		tex.reset(new GXTexture2D());
		tex->init(difftextureID, GXTexture2DType::GX_DIFFUSE);
		Buffer->addTexture(tex);
		tex.reset(new GXTexture2D());
		tex->init(hmaptextureID, GXTexture2DType::GX_HEIGHT);
		Buffer->addTexture(tex);
		std::shared_ptr<GXMeshComponent> planeMeshComp;
		planeMeshComp.reset(new GXMeshComponent(GXID, Buffer));
		components.emplace_back(planeMeshComp);
		//verts.clear();//no need for the data anymore.
	}

	void GXPlane::update(GXFloat deltaTime)
	{
		for (auto& component : components) {
			component->update(deltaTime);
		}
		GXint32 distance;
		GXint32 levels;
		//LOD0 [ 0 - 16]
		//LOD1 [ 16 - 32]
		//LOD2 [ 32 - 64]
		//update and add data to instances
		if (EditorCamera::getInstance().cameraMoved == true) {
			EditorCamera::getInstance().cameraMoved = false;
			instances.clear();
			qt.getInstances(instances, centerOffset, clustringFactor);
			if (instances.size() > 0) {
				Buffer->bindDynamicBuffer();
				Buffer->updateDynamicBuffer(&instances[0], sizeof(PlaneInstanceData) * instances.size());
				Buffer->unbindDynamicBuffer();
			}
		}
	}

	void GXPlane::draw()
	{
		//For rendering as rendering happens in GXMeshComponent
		if (instances.size() > 0) {
			this->shader->use();
			SceneLightManager::getInstance().setLightValues(this->shader);
			this->shader->setMat4("model", transform.getModel());
			this->shader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
			this->shader->setFloat("height", this->height);
			this->shader->setFloat("totalWidth", this->totalVertsX-1.0f);
			this->shader->setFloat("totalDepth", this->totalVertsZ-1.0f);
			for (auto& component : components) {
				component->draw(this->shader, RenderType::GX_TRIANGLES, isWireFrame, instances.size());
			}
		}
		
	}

	void GXPlane::destroy()
	{
		for (GXint32 i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}



}