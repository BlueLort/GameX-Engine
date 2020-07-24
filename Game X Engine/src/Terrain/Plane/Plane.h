#pragma once
#include "Config.h"
#include "Object/GXObject.h"
#include "Terrain/HeightGenerators/HeightMapGenerator.h"
#include "QuadTree.h"
namespace gx {
	class GX_DLL GXPlane : public GXObject {
	public:
			GXPlane(GXint32 PatchWidth, GXint32 PatchDepth, GXint32 Height,
				 GXint32 RowInstances, GXint32 ColInstances,
				GXint32 DensityX = 2, GXint32 DensityZ = 2, const char* shaderPath = nullptr) : GXObject(GX_PLANE,"Plane"),
				width(PatchWidth), depth(PatchDepth), clustringFactor(DensityX * PatchWidth, DensityZ * PatchDepth),
				rowInstances(RowInstances), colInstances(ColInstances), nInstances(RowInstances* ColInstances), 
				height(Height),indices(nullptr)
			{
			if (shaderPath == nullptr) {
				shader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_PLANEGBUFFER);
			}
			else {
				shader = GXShaderManager::getShader(shaderPath);
			}
			
				maxLOD = sqrt(sqrt(nInstances));
				overallWidth = colInstances * width;
				overallDepth = rowInstances * depth;
				totalVertsX = overallWidth * DensityX;
				totalVertsZ = overallDepth * DensityZ;
				centerOffset.x = colInstances * width / 2.0f;
				centerOffset.y = rowInstances * depth / 2.0f;
				qt.build(maxLOD, glm::vec3(0.0f, 0.0f, 0.0f), glm::uvec2(overallWidth, overallDepth));
				isWireFrame = true;
		}
		void init(const char* fileName, const char* shaderPath = nullptr){}
		void init();
		void uploadToBuffer(GXuint32 difftextureID, GXuint32 hmaptextureID);
		virtual void update(GXFloat deltaTime) override;
		virtual void draw() override;
		virtual void destroy() override;
		virtual void setHeight(GXint32 Height) { height = Height; }
		~GXPlane() { if (indices != nullptr)delete[] indices; }
	protected:
		GXint32 width;
		GXint32 depth;
		GXint32 height;
		GXuint32* indices;
		GXuVec2 clustringFactor;//Clustering factor in the XZ
		GXint32 totalIndicesSize;
		GXint32 nInstances, rowInstances, colInstances;
		GXint32 maxLOD;
		GXint32	overallWidth, overallDepth;
		GXint32 totalVertsX, totalVertsZ;
		std::vector<PlaneVertex3D> verts;
		std::vector<PlaneInstanceData> instances;
		QuadTree qt;
		GXVec2 centerOffset;
		std::shared_ptr<GXGraphicsBufferManager> Buffer;
	};
}