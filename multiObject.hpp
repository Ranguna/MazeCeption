#ifndef MOBJECT_H
#define MOBJECT_H

#include "object.hpp"
#include "fisiqs.hpp"

namespace Gameplay {
	class MultiObject : public Object {
		private:
			std::vector<fisiqs::FisiBody*> rigidBodies;

		public:
			MultiObject();
			MultiObject(float shininess, float specularStrength, glm::vec3 specularColor);
			MultiObject(std::vector<fisiqs::FisiBody*> rigidBodies);
			MultiObject(const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor);
			MultiObject(std::vector<fisiqs::FisiBody*> rigidBodies, const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor);
			
			bool areBodiesEnabled = true;

			void attachRigidBody(fisiqs::FisiBody* rigidBody);
			void attachRigidBodies(std::vector<fisiqs::FisiBody*> *rigidBodies);

			void disableRigidBodies();
			void enableRigidBodies();

			~MultiObject();
	};

}

#endif