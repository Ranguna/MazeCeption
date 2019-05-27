#include "multiObject.hpp"

namespace Gameplay {
	MultiObject::MultiObject()
	: Object() { }
	MultiObject::MultiObject(const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor)
	: Object(obj, vertexColor, shininess, specularStrength, specularColor) { }
	MultiObject::MultiObject(float shininess, float specularStrength, glm::vec3 specularColor)
	: Object(shininess, specularStrength, specularColor) { }

	MultiObject::MultiObject(std::vector<fisiqs::FisiBody*> rigidBodies)
	: Object() {
		attachRigidBodies(&rigidBodies);
	}

	MultiObject::MultiObject(std::vector<fisiqs::FisiBody*> rigidBodies, const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor)
	: Object(obj, vertexColor, shininess, specularStrength, specularColor){
		attachRigidBodies(&rigidBodies);
	}

	void MultiObject::attachRigidBody(fisiqs::FisiBody *rigidBody){
		rigidBodies.push_back(rigidBody);
	}
	void MultiObject::attachRigidBodies(std::vector<fisiqs::FisiBody*> *rigidBodies){
		this->rigidBodies.insert(this->rigidBodies.end(), rigidBodies->begin(), rigidBodies->end());
	}

	void MultiObject::disableRigidBodies(){
		if(!areBodiesEnabled)
			throw "Trying to disable already disabled bodies in MultiObject";

		for(std::vector<fisiqs::FisiBody*>::iterator it = rigidBodies.begin(); it != rigidBodies.end(); it++)
			(*it)->disableBody();
	}
	void MultiObject::enableRigidBodies(){
		if(areBodiesEnabled)
			throw "Trying to enable already enabled bodies in MultiObject";

		for(std::vector<fisiqs::FisiBody*>::iterator it = rigidBodies.begin(); it != rigidBodies.end(); it++)
			(*it)->enableBody();
	}

	MultiObject::~MultiObject(){
		if(rigidBodies.size() == 0)
			return;

		std::cout<< "Destructuring MultiObject"<<std::endl;
		for(int i = 0; i < rigidBodies.size(); i++){
			rigidBodies.at(0)->~FisiBody();
		}
		rigidBodies.clear();
	}
}
