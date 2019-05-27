#include "singleObject.hpp"

namespace Gameplay {
	void SingleObject::initEmpty(){
		rigidBody = NULL;
	}

	SingleObject::SingleObject()
	: Object() {
		initEmpty();
	}
	SingleObject::SingleObject(const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor)
	: Object(obj, vertexColor, shininess, specularStrength, specularColor) {
		initEmpty();
	}

	SingleObject::SingleObject(fisiqs::FisiBody *rigidBody)
	: Object() {
		attachRigidBody(rigidBody);
	}
	SingleObject::SingleObject(fisiqs::FisiBody *rigidBody, const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor)
	: Object(obj, vertexColor, shininess, specularStrength, specularColor){
		attachRigidBody(rigidBody);
	}

	void SingleObject::attachRigidBody(fisiqs::FisiBody *rigidBody){
		std::cout<<"attaching rb"<<std::endl;
		this->rigidBody = rigidBody;
	}

	SingleObject::~SingleObject(){
		std::cout << "rb: " << rigidBody<<std::endl;
		if(rigidBody == NULL)
			return;
		std::cout<< "Destructuring SingleObject"<<std::endl;
		rigidBody->~FisiBody();
	}
}