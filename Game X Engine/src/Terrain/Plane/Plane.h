#pragma once
#include "Config.h"
#include "Object/GXObject.h"
#include "Terrain/HeightGenerators/HeightMapGenerator.h"
namespace gx {
	class GX_DLL GXPlane : public GXObject {
	public:
		GXPlane(GXint32 Width, GXint32 Depth, GXint32 ClusteringX = 2, GXint32 ClusteringZ = 2, const char* shaderPath = nullptr) : GXObject(GX_PLANE,"Plane"), width(Width), depth(Depth), n(ClusteringX* Width), m(ClusteringZ* Depth), indices(nullptr) {
			if (shaderPath == nullptr) {
				glshader = GLShaderManager::getShader(GLShaderType::DEFAULT_GBUFFER);
			}
			else {
				glshader = GLShaderManager::getShader(shaderPath);
			}
		}
		void init(const float* heights);
		void uploadToBuffer(GXuint32 textureID);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
		~GXPlane() { if (indices != nullptr)delete[] indices; }
	protected:
		GXint32 width;
		GXint32 depth;
		GXint32 n;//Clustering factor in the X
		GXint32 m;//Clustering factor in Z
		GXuint32* indices;
		std::vector<Vertex3D> verts;
	private:
		inline GXVec3 getNormalVec(GXint32 idx1, GXint32 idx2, GXint32 idx3)
		{
			//order defines the output normal.[up or down]
			GXVec3 v1 = verts[idx3].position - verts[idx2].position;
			GXVec3 v2 = verts[idx1].position - verts[idx2].position;
			return GXMaths::normalize(GXMaths::cross(v1, v2));
		}
		inline float getHeightValue(float x, float z, const float* heights)
		{
			///  ver1---->interpolation1<----- ver2
			///          float place 
			///        
			///  ver3---->interpolation2<-----ver4
			GXint32 intX = static_cast<GXint32>(x);
			GXint32 intZ = static_cast<GXint32>(z);
			float floatX = abs(x - intX);
			float floatZ = abs(z - intZ);
			GXint32 nextX = intX < (width - 1) ? intX + 1 : intX;
			GXint32 nextZ = intZ < (depth - 1) ? intZ + 1 : intZ;
			GXint32 loc1 = intZ * width + intX;
			GXint32 loc2 = intZ * width + nextX;
			GXint32 loc3 = nextZ * width + intX;
			GXint32 loc4 = nextZ * width + nextX;
			float h1 = heights[loc1];
			float h2 = heights[loc2];
			float h3 = heights[loc3];
			float h4 = heights[loc4];
			float interpolant1 = GXMaths::cosinterp(floatX, h1, h2);
			float interpolant2 = GXMaths::cosinterp(floatX, h3, h4);
			return  GXMaths::cosinterp(floatZ, interpolant1, interpolant2);
		}
	};
}