#include "gameplay.hpp"

namespace Gameplay {
	Game::Game(Camera *camera, fisiqs::FisiWorld *fisiWorld, int n_mazes, int start_difficulty, int step){
		this->camera = camera;
		this->fisiWorld = fisiWorld;

		// load default shaders
		this->loadShaders("phong_illumination", "phong_illumination.vertexshader", "phong_illumination.fragmentshader");

		// generate mazes
		generateMazes(n_mazes, start_difficulty, step);
	}

	void Game::generatePlayerObject(glm::vec3 playerColor, float shininess, float specularStrength, glm::vec3 specularColor){
		playerSphere = new SingleObject("sphere.obj", playerColor, shininess, specularStrength, specularColor);
	}
	void Game::generatePlayerObject(fisiqs::FisiBody *rigidBody, glm::vec3 playerColor, float shininess, float specularStrength, glm::vec3 specularColor){
		generatePlayerObject(playerColor, shininess, specularStrength, specularColor);
		playerSphere->attachRigidBody(rigidBody);
	}

	void Game::movePlayerToCurrentStart(){
		playerSphere->getFisiBody()->setPosition(btVector3(
			2*(-getCurrentMaze()->initial_cell + getCurrentMaze()->size),
			-8*currentMaze,
			2*(getCurrentMaze()->size)
		));
	}

	void Game::newMaze(int difficulty, bool activated){
		// easiest difficulty, 0, is a 6x6*2 maze
		// difficulty will increase in steps of four, to a maximum of  ~I N F I N I T Y~ (roughly 4294967295)
		// boards larger than 16x16 will crash the game, though
		unsigned int size = 6+difficulty*2;

		// generate maze	
		Maze *maze = new Maze(size);
		std::cout << "freq "<< difficulty << ", " <<1.0-difficulty/9.0<<std::endl;
		// maze->build_maze_matrix(0);
		maze->build_maze_matrix(.5-difficulty/9.0);
		maze->generate_maze_coordinates();

		// create maze object
		MultiObject *mazeObj = new MultiObject(32.0, 0.15f, glm::vec3(1,1,1));

		// create rigidBodies
		int mazei = mazeObjects.size();
		for(int i=0; i < maze->size*2 +1; i++){
			for(int j=0; j < maze->size*2 +1; j++){
				if(maze->Matrix()[i][j] == 'X'){
					std::cout << "new body " << i*(maze->size*2)+1+j <<  std::endl;
					std::cout << "@(" << -j*2 << ", " << 0 - 8*mazei << ", " << -i*2 << ")" << std::endl;
					mazeObj->attachRigidBody(fisiWorld->createFisiBody(0, btVector3(-j*2.0 + maze->size*2, 0 - 8*mazei, -i*2.0 + maze->size*2), fisiWorld->createBoxShape(btVector3(1.,1.,1.))));
				}
				if(maze->Matrix()[i][j] == ' '){
					std::cout << "new floor body " << i*(maze->size*2)+1+j <<  std::endl;
					std::cout << "@(" << -j*2 << ", " << -2 - 8*mazei << ", " << -i*2 << ")" << std::endl;
					mazeObj->attachRigidBody(fisiWorld->createFisiBody(0, btVector3(-j*2.0 + maze->size*2, -2 - 8*mazei, -i*2.0 + maze->size*2), fisiWorld->createBoxShape(btVector3(1.,1.,1.))));
				}
			}
		}
		if(!activated)
			mazeObj->disableRigidBodies();

		// generate maze colors
		GLfloat mazeColors[maze->tot_coordinates];
		for(int i =0; i < maze->tot_coordinates; i+=9){
			if(maze->cube_coordinates[i+1] == -1){
				mazeColors[i] = 1.0f;
				mazeColors[i+1] = 1.0f;
				mazeColors[i+2] = 1.0f;
			}else{
				mazeColors[i] = 1.0f;
				mazeColors[i+1] = 0.0f;
				mazeColors[i+2] = 0.0f;
			}
			if(maze->cube_coordinates[i+4] == -1){
				mazeColors[i+3] = 1.0f;
				mazeColors[i+4] = 1.0f;
				mazeColors[i+5] = 1.0f;
			}else{
				mazeColors[i+3] = 0.0f;
				mazeColors[i+4] = 1.0f;
				mazeColors[i+5] = 0.0f;
			}
			if(maze->cube_coordinates[i+7] == -1){
				mazeColors[i+6] = 1.0f;
				mazeColors[i+7] = 1.0f;
				mazeColors[i+8] = 1.0f;
			}else{
				mazeColors[i+6] = 0.0f;
				mazeColors[i+7] = 0.0f;
				mazeColors[i+8] = 1.0f;
			}
		}

		mazeObj->loadVAOVBO(&maze->cube_coordinates[0], NULL, &maze->cube_normals[0], mazeColors, maze->tot_coordinates);

		// add maze object to maze object list
		mazeObjects.push_back(mazeObj);
		// add maze to maze list
		mazeList.push_back(maze);
		// set time at maze to 0
		mazeTimes.push_back(0);

		currentMaze = currentMaze == -1? 0 : currentMaze;
	}

	void Game::generateMazes(int n, int start_difficulty, int step){
		for(int i = 0; i < n; i++){
			newMaze(start_difficulty+step*i, i==0);
			std::cout<<"maze si " << mazeObjects.size() << std::endl;
			mazeObjects.at(i)->model = glm::translate(mazeObjects.at(i)->model, glm::vec3(0,-8*i,1));
		}
	}

	GLuint Game::loadShaders(const char *name, const char *vertex, const char *fragment){
		GLuint shaderId = LoadShaders(vertex, fragment);

		shaders.insert(pair<const char *, int>(name, shaderId));

		return shaderId;
	}

	Light *Game::newLight(){
		Light light = Light();
		lights.push_back(light);

		return &lights.at(lights.size()-1);
	}
	Light *Game::newLight(glm::vec3 position, glm::vec3 color, glm::vec3 ambientColor, float intensity, float ambientCoefficient){
		Light light = Light(position, color, ambientColor, intensity, ambientCoefficient);
		lights.push_back(light);

		return &lights.at(lights.size()-1);
	}

	void Game::endGame(bool won){
		std::cout << won << std::endl;
		std::cout << (won?"won":"lost") << std::endl;
		gameState = won? 1 : 0;
	}

	void Game::nextMaze(int offset){
		if(gameState > -1) // game has finished
			return;
		// std::cout << "Offset of: "<< offset << " cm "<< currentMaze << " maze size: "<< mazeObjects.size() << " won " <<(currentMaze + offset < 0)<< " || "<< (currentMaze + offset >= mazeObjects.size())<< std::endl;
		// std::cout << "cm: "<<  currentMaze << " offset " << offset << " size: "<< mazeObjects.size() << std::endl;
		int nz = currentMaze + offset;
		int s = mazeObjects.size();
		if(nz >= s)
			return endGame(false);
		if(nz < 0)
			return endGame(true);

		std::cout << "disabling bodies for current maze"<< std::endl;
		getCurrentMazeObject()->disableRigidBodies();
		currentMaze += offset;
		std::cout << "enabling bodies for current maze"<< std::endl;
		getCurrentMazeObject()->enableRigidBodies();
		std::cout << "moving player"<< std::endl;
		movePlayerToCurrentStart();
	}

	void Game::checkBoundery(){
		if(gameState > -1) // game has finished
			return;
		// std::cout << "pos (" << this->playerSphere->getFisiBody()->getWorldPosition().x << ", "<<this->playerSphere->getFisiBody()->getWorldPosition().y<<", "<<this->playerSphere->getFisiBody()->getWorldPosition().z <<"). " << (-this->playerSphere->getFisiBody()->getWorldPosition().z > getCurrentMaze()->size*2) <<std::endl;
		if(this->playerSphere->getFisiBody()->getWorldPosition().y < currentMaze*-8-0.5f)
			nextMaze(1);
		if(-this->playerSphere->getFisiBody()->getWorldPosition().z > getCurrentMaze()->size*2)
			nextMaze(-1);
		
	}

	void Game::update(double dt){
		// std::cout << 1/dt << std::endl;
		// update physics
		if(isFisiqsRunning)
			fisiWorld->update(dt);

		if(isGameRunning){
			// update time since launch
			runningTime += dt;
			checkBoundery();

			// update time in current maze
			if(currentMaze != -1)
				mazeTimes.at(currentMaze) += dt;
		}
	}

	void Game::draw(glm::mat4 *projection, const char *shaderName){
		GLuint shader = shaders.at(shaderName);

		// draw player
		playerSphere->draw(projection, camera->View, &lights.at(0), &camera->pos, shader);

		// draw mazes
		if(currentMaze != -1){
			for(int i = currentMaze; i < mazeObjects.size(); i++){
				mazeObjects.at(i)->draw(projection, camera->View, &lights.at(0), &camera->pos, shader);
			}
		}
		// for(std::vector<MultiObject*>::iterator it = mazeObjects.begin(); it != mazeObjects.end(); ++it){
		// 	(*it)->draw(projection, camera->View, &lights.at(0), &camera->pos, shader);
		// }
	}

	Game::~Game(){
		std::cout<< "Destructuring Game"<<std::endl;
		for(int i = 0; i < mazeObjects.size(); i++){
			delete mazeObjects.at(i);
		}
		mazeObjects.clear();
		std::cout<< "shader size: "<< shaders.size()<<std::endl;
		for(std::map<const char *, int>::iterator it = shaders.begin(); it != shaders.end(); it++){
			std::cout<< "delete shader " << it->first<<std::endl;
			glDeleteProgram(it->second);
		}
		delete playerSphere;
	}

}