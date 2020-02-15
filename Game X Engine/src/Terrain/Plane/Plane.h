#pragma once
#include "Config.h"
#include "Object/GXObject.h"
#include "Terrain/HeightGenerators/HeightMapGenerator.h"
namespace gx {
	class GX_DLL GXPlane : public GXObject {
	public:
		GXPlane(int Width, int Depth, int ClusteringX = 2, int ClusteringZ = 2, const char* shaderPath = nullptr) : GXObject(GX_PLANE), width(Width), depth(Depth), n(ClusteringX* Width), m(ClusteringZ* Depth), indices(nullptr) {
			if (shaderPath == nullptr) {
				glshader = GLShaderManager::getShader(GLShaderType::DEFAULT_PLANE);
			}
			else {
				glshader = GLShaderManager::getShader(shaderPath);
			}
		}
		void init(const float* heights);
		void uploadToBuffer(uint32_t textureID);
		virtual void update(float deltaTime) override;
		virtual void destroy() override;
		~GXPlane() { if (indices != nullptr)delete[] indices; }
	protected:
		int width;
		int depth;
		int n;//Clustering factor in the X
		int m;//Clustering factor in Z
		uint32_t* indices;
		std::vector<Vertex3D> verts;
	private:
		inline GXVec3 getNormalVec(int idx1, int idx2, int idx3)
		{
			//order defines the output normal.[up or down]
			GXVec3 v1 = verts[idx3].position - verts[idx2].position;
			GXVec3 v2 = verts[idx1].position - verts[idx2].position;
			return GXMaths::normalize(GXMaths::cross(v1, v2));
		}
		inline float getHeightValue(float x, float z, int locX, int locZ, const float* heights)
		{
			///  ver1---->interpolation1<----- ver2
			///          float place 
			///        
			///  ver3---->interpolation2<-----ver4
			int intX = static_cast<int>(x);
			int intZ = static_cast<int>(z);
			float floatX = abs(x - intX);
			float floatZ = abs(z - intZ);
			int nextX = locX < (width - 1) ? locX + 1 : locX;
			int nextZ = locZ < (depth - 1) ? locZ + 1 : locZ;
			int loc1 = locZ * width + locX;
			int loc2 = locZ * width + nextX;
			int loc3 = nextZ * width + locX;
			int loc4 = nextZ * width + nextX;
			float h1 = heights[loc1];
			float h2 = heights[loc2];
			float h3 = heights[loc3];
			float h4 = heights[loc4];
			float lerp1 = GXMaths::lerp(floatX, h1, h2);
			float lerp2 = GXMaths::lerp(floatX, h3, h4);
			return  GXMaths::lerp(floatZ, lerp1, lerp2);
		}
	};
}