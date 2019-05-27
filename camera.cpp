#include "camera.hpp"


Camera::Camera(glm::mat4 *view) {
	View = view;
	update();
}
Camera::Camera(glm::mat4 *view, glm::vec3 initialPosition) {
	View = view;
	pos = initialPosition;
	update();
}
Camera::Camera(glm::mat4 *view, glm::vec3 initialPosition, float initialPitch, float initialYaw) {
	View = view;
	pos = initialPosition;
	pitch = initialPitch;
	yaw = initialYaw;
	update();
}
void Camera::update() {
	(*View) = glm::lookAt(pos, pos + DirFront, DirUp);
}

void Camera::move(glm::vec3 offset){
	pos += offset*moveSensivity;
}
void Camera::move(GLfloat xoffset, GLfloat yoffset, GLfloat zoffset){
	// pos += glm::vec3(xoffset, yoffset, zoffset);
	pos.x += xoffset*moveSensivity;
	pos.y += yoffset*moveSensivity;
	pos.z += zoffset*moveSensivity;
}

void Camera::rotate(glm::vec2 offset){
	rotate(offset.x, offset.y);
}
void Camera::rotate(GLfloat xoffset, GLfloat yoffset){
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	DirFront = glm::normalize(newFront);
}
