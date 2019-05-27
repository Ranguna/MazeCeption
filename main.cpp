// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <ctime>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// GLM header file
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// shaders header file
#include <common/shader.hpp>
#include <common/objloader.hpp>

#include "camera.hpp"
#include "gameplay.hpp"
#include "maze.hpp"
#include "fisiqs.hpp"
#include "light.hpp"

vec2 getMousePosition(GLFWwindow *window){
	double x,y;
	glfwGetCursorPos(window,&x, &y);

	return vec2(x,y);
}

void process_input(GLFWwindow*window, Camera *camera, float speed, vec2 &lastMousePos, fisiqs::FisiWorld &fisiWorld, vec2 windowDimensions, bool &cameraUpdated, float dt){
	glm::vec3 moveOffset(0);
	glm::vec2 rotationOffset(0);

	if(glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
		cameraUpdated = true;
		// moveOffset.y += 0.1;
		moveOffset += camera->front()*speed*dt;
	}
	if(glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
		cameraUpdated = true;
		// moveOffset.y += -0.1;
		moveOffset += -camera->front()*speed*dt;
	}
	if(glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
		cameraUpdated = true;
		// moveOffset.z += 0.1;
		moveOffset -= glm::normalize(glm::cross(camera->front(), camera->up()))*speed*dt;
	}
	if(glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
		cameraUpdated = true;
		// moveOffset.z += -0.1;
		moveOffset += glm::normalize(glm::cross(camera->front(), camera->up()))*speed*dt;
	}
	if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
		// moveOffset.y += 0.1;
	}

	if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		// moveOffset.y += 0.1;
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		cameraUpdated = true;
		rotationOffset.x += -0.01*speed*dt;
	}
	if(glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		cameraUpdated = true;
		rotationOffset.x += 0.01*speed*dt;
	}
	if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
		cameraUpdated = true;
		
		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS)
			moveOffset = camera->up();
		else
			rotationOffset.y += 0.01*speed*dt;
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		cameraUpdated = true;

		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS)
			moveOffset = -camera->up();
		else
			rotationOffset.y += -0.01*speed*dt;
	}

	// mouse input
	vec2 currentMousePos = getMousePosition(window);
	if(currentMousePos != lastMousePos){
		vec2 gravity = currentMousePos - vec2(windowDimensions.x/2, windowDimensions.y/2);
		float distanceFromCenter = std::min(length(gravity)/length(windowDimensions/vec2(2)),1.0f);
		// std::cout << "distance: " << distanceFromCenter << std::endl;
		gravity = normalize(gravity);
		// std::cout << "New gravity, old:" << gravity.x*5*distanceFromCenter << ", "<< gravity.y*5*distanceFromCenter << std::endl;
		fisiWorld.getWorld()->setGravity(btVector3(gravity.x*(3.14159265*7.25)*.8*distanceFromCenter,-10,gravity.y*(3.14159265*7.25)*.8*distanceFromCenter));
		lastMousePos = currentMousePos;
	}

	if(cameraUpdated){
		camera->rotate(rotationOffset);
		camera->move(moveOffset);
	}
}

//--------------------------------------------------------------------------------
Camera setupCamera(glm::mat4 *Projection, glm::mat4 *View, glm::vec3 initialPosition, float initialPitch, float initialYaw){
	// Esta é uma variavel dentro da vertex shader
	// Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	*Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
		
	// Camera matrix
	*View = glm::lookAt(
		initialPosition, // Camera is at (4,3,-3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	Camera camera = Camera(View, initialPosition, initialPitch, initialYaw);

	camera.rotate(0.0f,0.0f);
	camera.update();

	return camera;
 }

int main( void ){

	// Initialise GLFW
	glfwInit();
		
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	GLint WindowWidth = 1024;
	GLint WindowHeight = 768;
	vec2 windowDimensions(WindowWidth, WindowHeight);
	// Open a window
	GLFWwindow *window = glfwCreateWindow( WindowWidth, WindowHeight, "Minimal example", NULL, NULL);
		
	// Create window context
	glfwMakeContextCurrent(window);
		
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	glewInit();
		
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		
	// Black background
	glClearColor(19/255.0, 9/255.0, 22/255.0, 0.0f);
	// glClearColor(47/255.0, 24/255.0, 56/255.0, 0.0f);
		
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT );
		
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// prepare physics
	// create world with given gravity
	fisiqs::FisiWorld fisiWorld(btVector3(0,-10,0));
	fisiqs::FisiBody* sphereBody = fisiWorld.createFisiBody(1, btVector3(-1.25, 10.0, -3), fisiWorld.createSphereShape(0.75));

	// set the model-view-projection matrix
	glm::mat4 Projection(0);
	glm::mat4 View(0);
	Camera camera = setupCamera(&Projection, &View, glm::vec3(0,40,0.5), -89, 270);

	// initiate game
	Gameplay::Game game(&camera, &fisiWorld);

	// generate 1 maze
	game.generateMazes(10);

	// create player body
	game.generatePlayerObject(sphereBody, vec3(1.,.5,0.), 1.0f, 0.15f, vec3(1,1,1));
	game.getPlayer()->getFisiBody()->setPosition(btVector3(2*(-game.getCurrentMaze()->initial_cell + game.getCurrentMaze()->size), 0, 2*(game.getCurrentMaze()->size)));
	

	// add one light
	Gameplay::Light *l = game.newLight(camera.pos, vec3(1.0f,1.0f,1.0f), vec3(19/255.0, 9/255.0, 22/255.0), 0.0005f, 0.3f);

	// load phong shaders
	game.loadShaders("phong illumination", "phong_illumination.vertexshader", "phong_illumination.fragmentshader");

	bool cameraUpdated = false;
	auto last_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();
	vec2 lastMousePos = getMousePosition(window);
	double dt;
	float x,z;
	// render scene for each frame
	do{
		// get delta time
		current_time  = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<double, std::milli>(current_time-last_time).count()/1000.0;
		last_time = current_time;
		// std::cout << 1.0/dt << " FPS with " << dt << std::endl;

		// update game
		game.update(dt);

		process_input(window, &camera, 50, lastMousePos, fisiWorld, windowDimensions, cameraUpdated, dt);
		if(cameraUpdated){
			camera.update();
			cameraUpdated = false;
		}

		x = fisiWorld.getWorld()->getGravity().z();
		z = -fisiWorld.getWorld()->getGravity().x();
		
		// Process sphere transformations (the sphere does not rotate around itself)
		game.getPlayer()->model = glm::rotate(glm::mat4(1), glm::radians(x), vec3(1,0,0));
		game.getPlayer()->model = glm::rotate(game.getPlayer()->model, glm::radians(z), vec3(0,0,1));
		game.getPlayer()->model = glm::translate(game.getPlayer()->model, sphereBody->getWorldPosisiton());
		game.getPlayer()->model = glm::scale(game.getPlayer()->model, vec3(.038,.038,.038));

		// Rotate board
		game.getCurrentMazeObject()->model = glm::rotate(glm::mat4(1), glm::radians(x), vec3(1,0,0));
		game.getCurrentMazeObject()->model = glm::rotate(game.getCurrentMazeObject()->model, glm::radians(z), vec3(0,0,1));

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update light position to camera
		l->position = camera.pos;
		
		// draw game
		game.draw(&Projection, "phong illumination");
		
		// // draw final cell
		// cubeModel = glm::translate(glm::mat4(1), vec3(2*(-maze.final_cell+maze.size), 0, -2*(maze.size)));
		// draw(programID, &camera, Projection, &objects.at(2));
		// // draw initial cell
		// cubeModel = glm::translate(glm::mat4(1), vec3(2*(-maze.initial_cell+maze.size), 0, 2*(maze.size)));
		// draw(programID, &camera, Projection, &objects.at(2));
		
		// Swap buffers
		glfwSwapBuffers(window);
		// looking for events
		glfwPollEvents();
	} while(
		glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0
	); // Check if the ESC key was pressed or the window was closed
		// std::abs(objects.at(0).rigidBody->getWorldPosisiton().z) < maze.size*2-2-0.5

	// Cleanup VAO, VBOs, and shaders from GPU
	// cleanupDataFromGPU(programID, &objects);
		
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
		
	return 0;
}
