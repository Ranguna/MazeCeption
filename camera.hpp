#ifndef CAM_GUARD
#define CAM_GUARD

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

class Camera {
	private:
		float pitch = 0.0f;
		float yaw = 180.0f;

		//camera stuff
		glm::vec3 DirFront = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 DirUp = glm::vec3(0.0f, 1.0f, 0.0f);

	public:
		glm::mat4 *View;
		glm::vec3 pos = glm::vec3(-20.0f, -1.0f, -20.0f);
		// glm::vec3 pos = glm::vec3(6.0f, 0.0f, 0.0f);
		float moveSensivity = 0.1;
		float sensitivity = 100.0f;

		glm::vec3 front() const {
			return DirFront;
		}
		glm::vec3 up() const {
			return DirUp;
		}
		glm::mat4 *view() {
			return View;
		}

		void update();

		void move(glm::vec3 offset);
		void move(GLfloat xoffset, GLfloat yoffset, GLfloat zoffset);

		void rotate(glm::vec2 offset);
		void rotate(GLfloat xoffset, GLfloat yoffset);

		Camera (glm::mat4 *view);
		Camera (glm::mat4 *view, glm::vec3 initialPosition);
		Camera (glm::mat4 *view, glm::vec3 initialPosition, float initialPitch, float initialYaw);
};

#endif
