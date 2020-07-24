#pragma once
#include "pch.h"
#include "Config.h"
#include "Maths/GXMaths.h"
#include "Camera/EditorCamera/EditorCamera.h"
namespace gx {

	static GXuint32 pow2table[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
	struct TerrainNode {
		GXuVec2 instanceID; // Corresponding instance id [grid location xz] [can be id of anyinstance that can cover whole patches]
		GXint32 LOD;// LOD supplied to the instance
		GXuVec2 size;// Overall size of the node
		GXVec3 nodePos;// Node position
		std::unique_ptr<TerrainNode> children[4];// Patches covered by this node
		void build(GXuVec2 ID, GXint32 maxLOD, GXint32 LOD, const GXVec3& pos, const GXuVec2 size) {
			this->instanceID = ID;
			this->LOD = LOD;
			this->nodePos = pos;
			this->size = size;
			if (LOD <= 0) {
				return;
			}
			GXint32 nextLOD = LOD - 1;
			children[0].reset(new TerrainNode());
			children[0]->build(this->instanceID, maxLOD, nextLOD, nodePos - GXVec3((size.x / 4.0f), 0.0f, (size.y / 4.0f))
				, GXuVec2((size.x / 2), (size.y / 2)));

			children[1].reset(new TerrainNode());
			children[1]->build(GXuVec2(this->instanceID.x + pow2table[nextLOD], this->instanceID.y), maxLOD, nextLOD, nodePos - GXVec3(-(size.x / 4.0f), 0.0f, (size.y / 4.0f))
				, GXuVec2((size.x / 2), (size.y / 2)));

			children[2].reset(new TerrainNode());
			children[2]->build(GXuVec2(this->instanceID.x, this->instanceID.y + pow2table[nextLOD]), maxLOD, nextLOD, nodePos - GXVec3((size.x / 4.0f), 0.0f, -(size.y / 4.0f))
				, GXuVec2((size.x / 2), (size.y / 2)));

			children[3].reset(new TerrainNode());
			children[3]->build(GXuVec2(this->instanceID.x + pow2table[nextLOD], this->instanceID.y + pow2table[nextLOD]), maxLOD, nextLOD, nodePos - GXVec3(-(size.x / 4.0f), 0.0f, -(size.y / 4.0f))
				, GXuVec2((size.x / 2), (size.y / 2)));

		}
		void getInstances(std::vector<PlaneInstanceData>& instances, const GXVec2& centerOffset, const GXuVec2& verticesCount) {
			// Node as a whole should be in frustum
			if (EditorCamera::getInstance().isInFrustum(this->nodePos, this->size) == false) {
				// Reject node as a whole
				return;
			}
			//if any one of the children in LOD RANGE all same lod at least.
			GXint32 ite;

			if (this->LOD == 0) {
				// i'm maximum resolution -> add current Node to drawn patches
				addCurrent(instances, centerOffset, verticesCount);
				return;
			}

			for (ite = 0; ite < 4; ite++) {
				if (children[ite]->isInLODRange(EditorCamera::getInstance().transform.position) == true)break;
			}
			if (ite == 4) {
				// All children nodes are out of LOD range  -> add current Node to drawn patches
				addCurrent(instances, centerOffset, verticesCount);
			}
			else {
				// At Least one node satisfies LOD range -> Try to Render all children nodes
				for (ite = 0; ite < 4; ite++) {
					children[ite]->getInstances(instances, centerOffset, verticesCount);
				}
			}
		}
		bool isInLODRange(const GXVec3& camPos) {
			GXint32 distance;
			GXint32 levels;
			//LOD0 [ 0 - 16]
			//LOD1 [ 16 - 32]
			//LOD2 [ 32 - 64]
			distance = glm::length(camPos - this->nodePos);
			distance >>= 4;//removing the 0to16 range
			distance >>= this->LOD;
			return distance == 0;
		}
		inline void addCurrent(std::vector<PlaneInstanceData>& instances, const GXVec2& centerOffset, const GXuVec2& verticesCount) {
			instances.emplace_back(
				GXVec2((this->instanceID.x * size.x / static_cast<float>(pow2table[this->LOD])) + (this->size.x / 2.0f) - centerOffset.x, (this->instanceID.y * size.y / static_cast<float>(pow2table[this->LOD])) + (this->size.y / 2.0f) - centerOffset.y)// POS offset
				, GXiVec2(this->instanceID.x * verticesCount.x, this->instanceID.y * verticesCount.y) // Sampling Offset
				, this->LOD); // LOD
		}

	};
	class QuadTree {
	public:
		QuadTree() {

		}
		void build(GXint32 LOD, const GXVec3& pos, const GXuVec2 size) {
			root.build(GXuVec2(0, 0), LOD, LOD, pos, size);
		}
		void getInstances(std::vector<PlaneInstanceData>& instances, const GXVec2& centerOffset, const GXuVec2& verticesCount) {
			root.getInstances(instances, centerOffset, verticesCount);
		}

	private:
		TerrainNode root;

	};
}