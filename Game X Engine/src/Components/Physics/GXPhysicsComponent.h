#pragma once
#include "Maths/GXMaths.h"
#include "Maths/GXTransform.h"
#include "Components/GXComponent.h"
#include <Bullet/btBulletDynamicsCommon.h>


namespace gx {
	class GX_DLL GXPhysicsComponent : public GXComponent {
	public:

		inline GXPhysicsComponent(GXuint32 GXID) : GXComponent(GXID)
		{
			//TODO Make all of this options controllable
			collisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

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


			rb->setUserPointer(reinterpret_cast<void*>(GXID));
			

		}
		virtual inline const char* getName() const override {
			return "GXPhysicsComponent";
		}
		virtual inline GXuint32 getEventClass() const override {
			return GXComponentClass::GX_PHYSICS;
		}
		virtual inline GXuint32 getEventType()const override {
			return GXComponentType::GX_COLLIDER | GXComponentType::GX_RIGIDBODY;
		}
		virtual void destroy()override {

		}
		virtual void update(float deltaTime)override {
			//TODO update the other details of owners data
			ownerTransform->position.x = transform.getOrigin().getX();
			ownerTransform->position.y = transform.getOrigin().getY();
			ownerTransform->position.z = transform.getOrigin().getZ();
			ownerTransform->rotation.x = transform.getRotation().getX();
			ownerTransform->rotation.y = transform.getRotation().getY();
			ownerTransform->rotation.z = transform.getRotation().getZ();
		}
		btCollisionShape* getShape()const { return collisionShape; }
		btRigidBody* getRigidBody()const { return rb; }
		void setOwnerTransform(GXTransform* ot) { ownerTransform = ot; }
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