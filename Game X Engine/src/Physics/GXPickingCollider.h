#pragma once
//implementation for a ray picking collision Shape
#include "Maths/GXMaths.h"
#include "Maths/GXTransform.h"
#include "Physics/PhysicsManager.h"
namespace gx {
	//Model Object is Object with RendererComponent and Assimp Loaded Object.
	class GX_DLL GXPickingCollider {
	public:
		//TODO FIX THIS CLASS TO BE A MOUSE PICKING CLASS INSTEAD OF COLOR PICKING
		inline GXPickingCollider(std::vector<Vertex3D>& verts) {
			//collisionShape = new btConvexHullShape();
			collisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
			int32_t size = verts.size();
			//for (int32_t i = 0; i < size; i++) {
			//	static_cast<btConvexHullShape*>(collisionShape)->addPoint(btVector3(verts[i].position.x, verts[i].position.y, verts[i].position.z));
			//}

			transform.setIdentity();//initially objects imported to 0,0,0

			mass = btScalar(0.0);//let mass be 1 for now

			bool isDynamic = false;//static for now

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				collisionShape->calculateLocalInertia(mass, localInertia);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			myMotionState = new btDefaultMotionState(transform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
			rb = new btRigidBody(rbInfo);

			GXPhysicsManager::getInstance().addRigidBody(rb);

		}
		btCollisionShape* getShape()const { return collisionShape; }
		void setOwnerTransform(GXTransform* ot) { ownerTransform = ot; }
		void setOwnerID(uint32_t GXID) {rb->setUserPointer(reinterpret_cast<void*>(GXID));}
	private:
		btCollisionShape* collisionShape;
		btDefaultMotionState* myMotionState;
		btRigidBody* rb;
		btTransform transform;
		bool isDynamic;
		btScalar mass;
		GXTransform* ownerTransform;

	};

}