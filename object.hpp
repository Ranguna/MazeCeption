#ifndef OBJECT_H
#define OBJECT_H

// standard library
#include <iostream>
#include <iterator>
#include <vector>

// GLEW
#include <GL/glew.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// glw
#include <common/shader.hpp>
#include <common/objloader.hpp>

// project related
#include "light.hpp"

namespace Gameplay {
	class Object {
		private:
			int vetexCount;
			GLuint VAO;
			GLuint VBO_vetex;
			GLuint VBO_normals;
			GLuint VBO_color;
			float shininess;
			float specularStrength;
			glm::vec3 specularColor;

			bool isObjLoaded = false;

		public:
			Object();
			Object(const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor);
			Object(float shininess, float specularStrength, glm::vec3 specularColor);

			glm::mat4 model;

			void loadObj(const char *obj, glm::vec3 color);
			void loadVAOVBO(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals, std::vector<glm::vec3> vertexColors);
			void loadVAOVBO(float *vertices, float *uvs, float *normals, float *vertexColors, int totVertices);

			virtual void draw(glm::mat4 *projection, glm::mat4 *view, Light *light, glm::vec3 *cameraPosition, GLuint shader);

			virtual ~Object();
	};

}

#endif