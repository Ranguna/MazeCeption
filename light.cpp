#include "light.hpp"

namespace Gameplay {
	Light::Light(glm::vec3 position, glm::vec3 color, glm::vec3 ambientColor, float intensity, float ambientCoefficient){
		this->position = position;
		this->color = color;
		this->ambientColor = ambientColor;
		this->intensity = intensity;
		this->ambientCoefficient = ambientCoefficient;
	}
}
