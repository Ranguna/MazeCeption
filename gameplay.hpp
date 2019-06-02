#ifndef GAMEPLAY_H
#define GAMEPLAY_H

// standard library
#include <vector>
#include <iterator>
#include <map>
#include <string>

// GLEW
#include <GL/glew.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>

// glw
#include <common/shader.hpp>
#include <common/objloader.hpp>

// project related
#include "singleObject.hpp"
#include "multiObject.hpp"
#include "maze.hpp"
#include "fisiqs.hpp"
#include "light.hpp"
#include "camera.hpp"

namespace Gameplay {
	class Game {
		private:
			std::vector<MultiObject*> mazeObjects;
			std::vector<Maze*> mazeList;
			SingleObject *playerSphere;

			std::vector<Light> lights;

			fisiqs::FisiWorld *fisiWorld;
			bool isFisiqsRunning = true;
			
			Camera *camera;

			bool isGameRunning = true;
			double runningTime = 0;
			std::vector<double> mazeTimes;

			std::map<const char *, int> shaders;

			void nextMaze(int offset);
			void checkBoundery();
			void endGame(bool won);
		public:
			Game(Camera *camera, fisiqs::FisiWorld *fisiWorld, int n_mazes=0, int start_difficulty=0, int step=1);

			int gameState = -1; // -1: ongoing, 0: lost, 1: won
			
			int currentMaze = -1;
			std::vector<MultiObject*>* getMazes(){ return &mazeObjects; }
			MultiObject *getCurrentMazeObject(){ return currentMaze != -1? mazeObjects.at(currentMaze) : NULL; }
			Maze *getCurrentMaze(){ return currentMaze != -1? mazeList.at(currentMaze) : NULL; }

			fisiqs::FisiWorld *getfisiWorld(){ return fisiWorld; }
			SingleObject *getPlayer(){ return playerSphere; }

			void pauseFisiqs(){ isFisiqsRunning = false; }
			void resumeFisiqs(){ isFisiqsRunning = true; }

			void generatePlayerObject(fisiqs::FisiBody *rigidBody, glm::vec3 playerColor, float shininess, float specularStrength, glm::vec3 specularColor);
			void generatePlayerObject(glm::vec3 playerColor, float shininess, float specularStrength, glm::vec3 specularColor);
			void movePlayerToCurrentStart();

			void newMaze(int difficulty, bool activated);
			void generateMazes(int n=1, int start_difficulty=0, int step=1);

			Light *newLight();
			Light *newLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambientColor, float intensity, float ambientCoefficient);

			GLuint loadShaders(const char *name, const char *vertex, const char *fragment);

			void update(double dt);
			void draw(glm::mat4 *projection, const char *shader);

			~Game();
			

	};
}
// for(std::vector<Object>::iterator it = objects->begin(); it != objects->end(); ++it){
#endif
