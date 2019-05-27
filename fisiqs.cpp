#include "fisiqs.hpp"

namespace fisiqs {
	glm::mat4 btScalar2glmMat4(btScalar* matrix) {
		return glm::mat4(
			matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]
		);
	}

	FisiWorld::FisiWorld(btVector3 gravity){
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		m_broadphase = new btDbvtBroadphase();
		m_solver = new btSequentialImpulseConstraintSolver;
		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

		m_dynamicsWorld->setGravity(gravity);
	}

	void FisiWorld::update(float dt){
		m_dynamicsWorld->stepSimulation(dt, 0.017);
	}

	btBoxShape* FisiWorld::createBoxShape(const btVector3& halfExtents){
		return new btBoxShape(halfExtents);
	}

	btSphereShape* FisiWorld::createSphereShape(const btScalar radius){
		return new btSphereShape(radius);
	}

	FisiBody* FisiWorld::createFisiBody(float mass, const btVector3& startPosition, btCollisionShape* shape, btVector3 localInertia){
		btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(startPosition);

		shape->calculateLocalInertia(mass, localInertia);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		if (isDynamic)
			shape->calculateLocalInertia(mass, localInertia);
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

		btRigidBody* body = new btRigidBody(cInfo);
		body->setActivationState(DISABLE_DEACTIVATION);
		//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

		body->setUserIndex(-1);
		m_dynamicsWorld->addRigidBody(body);

		return new FisiBody(body, this);
	}


	glm::mat4 FisiWorld::getBodyModel(btRigidBody *body){
		// btTransform trans = body->getWorldTransform();
		body->getMotionState()->getWorldTransform(aux_trans);
		
		aux_trans.getOpenGLMatrix(aux_transform);
		
		return btScalar2glmMat4(aux_transform);
	}


	FisiBody::FisiBody(){
		body = NULL;
		world = NULL;
	}
	FisiBody::FisiBody(btRigidBody *rb, FisiWorld *World){
		body = rb;
		world = World;
	}

	void FisiBody::setPosition(btVector3 position){
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(position);

		body->setWorldTransform(transform);
        body->getMotionState()->setWorldTransform(transform);

        body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
        body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
        body->clearForces();
	}

	glm::mat4 FisiBody::getOpenGLMatrix(){
		return world->getBodyModel(body);
	}

	glm::vec3 FisiBody::getWorldPosisiton(){
		// btTransform transform = body->getWorldTransform();
		btTransform transform;
		body->getMotionState()->getWorldTransform(transform);
		
		return glm::vec3(transform.getOrigin().x(),transform.getOrigin().y(),transform.getOrigin().z());
	}

	glm::vec3 FisiBody::getRotation(){
		// btTransform transform = body->getWorldTransform();
		btTransform transform;
		body->getMotionState()->getWorldTransform(transform);
		
		return glm::vec3(transform.getRotation().getAxis().x(),transform.getRotation().getAxis().y(),transform.getRotation().getAxis().z());
	}

	void FisiBody::disableBody(){
		if(isDisabled)
			throw "Trying to disable already disabled body";

		world->getWorld()->removeRigidBody(body);
		isDisabled = true;
	}
	void FisiBody::enableBody(){
		if(!isDisabled)
			throw "Trying to enable already enabled body";
		
		world->getWorld()->removeRigidBody(body);
		isDisabled = false;
	}

	FisiBody::~FisiBody(){
		if(body == NULL || world != NULL)
			return;

		world->getWorld()->removeRigidBody(body);
		delete body;
	}
}