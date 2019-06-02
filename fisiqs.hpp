#ifndef FISI_GUARD
#define FISI_GUARD

#include<iostream>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

namespace fisiqs {
	glm::mat4 btScalar2glmMat4asdasd(btScalar* matrix);

	class FisiWorld;

	class FisiBody {
		private:
			bool isDisabled = false;
		public:
			btRigidBody* body;
			FisiWorld* world;

			FisiBody();
			FisiBody(btRigidBody *rb, FisiWorld *world);

			void setPosition(btVector3 position);

			glm::mat4 getOpenGLMatrix();
			glm::vec3 getWorldPosition();
			glm::vec3 getRotation();
			void disableBody();
			void enableBody();

			~FisiBody();
	};

	class FisiWorld {
		private:
			btDefaultCollisionConfiguration* m_collisionConfiguration;
			btCollisionDispatcher* m_dispatcher;
			btBroadphaseInterface* m_broadphase;
			btConstraintSolver* m_solver;
			btDiscreteDynamicsWorld* m_dynamicsWorld;

			btScalar aux_transform[16];
			btTransform aux_trans;

		public:
			FisiWorld(btVector3 gravity);

			btDiscreteDynamicsWorld* getWorld() { return m_dynamicsWorld; }

			void update(float dt);

			FisiBody* createFisiBody(float mass, const btVector3& startPosition, btCollisionShape* shape, btVector3 localInertia = btVector3(0,0,0));
			btBoxShape* createBoxShape(const btVector3& halfExtents);
			btSphereShape* createSphereShape(const btScalar radius);

			glm::mat4 getBodyModel(btRigidBody *body);
	};
}

#endif