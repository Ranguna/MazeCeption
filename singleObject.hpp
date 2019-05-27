#ifndef SOBJECT_H
#define SOBJECT_H

#include "object.hpp"
#include "fisiqs.hpp"

namespace Gameplay {
	class SingleObject : public Object {
		private:
			fisiqs::FisiBody *rigidBody;

		public:
			SingleObject();
			SingleObject(fisiqs::FisiBody *rigidBody);
			SingleObject(const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor);
			SingleObject(fisiqs::FisiBody *rigidBody, const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor);
			void initEmpty();

			fisiqs::FisiBody *getFisiBody(){ return rigidBody; }

			void attachRigidBody(fisiqs::FisiBody *rigidBody);

			~SingleObject();
	};

}

#endif