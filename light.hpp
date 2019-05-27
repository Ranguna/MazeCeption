#ifndef LIGHT_H
#define LIGHT_H

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Gameplay {
	class Light {
		public:
			Light(glm::vec3 position = glm::vec3(0,0,0), glm::vec3 color = glm::vec3(1,1,1), glm::vec3 ambientColor = glm::vec3(19/255.0, 9/255.0, 22/255.0), float intensity = 0.002f, float ambientCoefficient = 0.3f);

			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 ambientColor;
			float intensity;
			float ambientCoefficient;
	};
}

#endif
