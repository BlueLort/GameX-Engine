#pragma once
#include "Config.h"
#include <Bullet/btBulletDynamicsCommon.h>

namespace gx {

	class GX_DLL GXPhysicsManager {
	public:

		void init();
		static GXPhysicsManager& getInstance() {
			static GXPhysicsManager instance;
			return instance;
		}
		inline void addCollisionShape(btCollisionShape* shape) { collisionShapes.push_back(shape); }
		inline void addRigidBody(btRigidBody* rb) { dynamicsWorld->addRigidBody(rb); }
		inline void stepSimulation(float timeStep, int maxSubSteps=1, float fixedTimeStep = 0.01666666f) { dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep); }
	
	private:
		GXPhysicsManager() {

		}
		~GXPhysicsManager();
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		std::vector<btCollisionShape*> collisionShapes;

	};
}