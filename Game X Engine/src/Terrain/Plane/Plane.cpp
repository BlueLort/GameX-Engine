#include "pch.h"
#include "Plane.h"

namespace gx {
	//heights must be bigger in width & height than plane
	void GXPlane::init(const float* heights)
	{
		verts.reserve(n * m);
		GXint32 nMinusOne = n - 1;
		GXint32 mMinusOne = m - 1;
		float dx = static_cast<float>(width) / n;
		float dz = static_cast<float>(depth) / m;

		/* i*dv or j *du alternative to get u&v
		float du = 1.0f / nMinusOne;
		float dv = 1.0f / mMinusOne;
		*/
		//Using topLeft to center Plane aroung origin
		float topLeftX = width / -2.0f;
		float topLeftZ = depth / -2.0f;
		GXint32 totalIndicesSize = nMinusOne * mMinusOne * 6;
		indices = new GXuint32[totalIndicesSize];

		for (GXuint32 i = 0; i < m; i++)
		{
			float z = i * dz;
			float posZ = topLeftZ + z;
			float v = static_cast<float>(i) / m;
			for (GXuint32 j = 0; j < n; j++)
			{
				float x = j * dx;
				float posX = topLeftX + x;
				float u = static_cast<float>(j) / n;
				verts.emplace_back(
					GXVec3(posX, getHeightValue(x, z, heights), posZ)// POS
					, GXVec2(u, v));// TEXCOORDS
			}
		}
		GXint32 k = 0;
		for (GXint32 z = 0; z < mMinusOne; z++)
		{
			for (GXint32 x = 0; x < nMinusOne; x++)
			{//TODO have a little redundency assigning normals here that needs to be improved
				GXVec3 normal;
				indices[k] = z * n + x;
				indices[k + 1] = (z + 1) * n + x;
				indices[k + 2] = z * n + x + 1;

				normal = getNormalVec(indices[k], indices[k + 1], indices[k + 2]);
				verts[indices[k]].normal = normal;
				verts[indices[k + 1]].normal = normal;
				verts[indices[k + 2]].normal = normal;

				indices[k + 3] = (z + 1) * n + x;
				indices[k + 4] = (z + 1) * n + x + 1;
				indices[k + 5] = z * n + x + 1;

				normal = getNormalVec(indices[k + 3], indices[k + 4], indices[k + 5]);
				verts[indices[k + 3]].normal = normal;
				verts[indices[k + 4]].normal = normal;
				verts[indices[k + 5]].normal = normal;

				k += 6; // next quad
			}
		}

	}

	void GXPlane::uploadToBuffer(GXuint32 textureID)
	{
		std::shared_ptr<GLBufferManager> Buffer;
		Buffer.reset(new GLBufferManager());
		Buffer->initFull(&verts[0], sizeof(Vertex3D) * verts.size(), sizeof(Vertex3D));
		GXuint32 totalIndicesSize = (n - 1) * (m - 1) * 6;
		Buffer->uploadIndicesToBuffer(indices, totalIndicesSize * sizeof(GXuint32), totalIndicesSize);
		Buffer->setAttribPointer(0, 3, GL_FLOAT, offsetof(Vertex3D, position));
		Buffer->setAttribPointer(1, 3, GL_FLOAT, offsetof(Vertex3D, normal));
		Buffer->setAttribPointer(2, 2, GL_FLOAT, offsetof(Vertex3D, texCoords));
		Buffer->setAttribPointer(3, 3, GL_FLOAT, offsetof(Vertex3D, tangent));
		Buffer->setAttribPointer(4, 3, GL_FLOAT, offsetof(Vertex3D, bitangent));
		//TODO Add Tangents bitangents later
		Buffer->endStream();
		/*for (GXint32 i = 0; i < textures.size(); i++) {
			Buffer->addTexture(textures[i]);
		}*/
		std::shared_ptr<GLTexture2D> tex;
		tex.reset(new GLTexture2D());
		tex->init(textureID, GXTexture2DType::GX_DIFFUSE);
		Buffer->addTexture(tex);
		std::shared_ptr<GXMeshComponent> planeMeshComp;
		planeMeshComp.reset(new GXMeshComponent(GXID,Buffer));
		components.emplace_back(planeMeshComp);
		verts.clear();//no need for the data anymore.
	}

	void GXPlane::update(float deltaTime)
	{
		//For rendering as rendering happens in GXMeshComponent
#ifdef USING_OPENGL
		this->glshader->use();
		SceneLightManager::getInstance().setLightValues(this->glshader);
		this->glshader->setMat4("model", transform.getModel());
		this->glshader->setMat4("vp", EditorCamera::getInstance().getPVMatrix());
#endif
		for (auto& component : components) {
			component->update(deltaTime);
			component->draw(glshader, isWireFrame);
		}
	}

	void GXPlane::destroy()
	{
		for (GXint32 i = 0; i < components.size(); i++) {
			components[i]->destroy();
		}
	}



}