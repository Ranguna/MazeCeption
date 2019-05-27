#include "object.hpp"

namespace Gameplay {
	Object::Object(){
		vetexCount = -1;
		VAO = -1;
		VBO_vetex = -1;
		VBO_normals = -1;
		VBO_color = -1;
		shininess = 0;
		specularStrength = 0;
		specularColor = glm::vec3(0,0,0);
		model = glm::mat4(1);
		isObjLoaded=false;
	}
	Object::Object(const char *obj, glm::vec3 vertexColor, float shininess, float specularStrength, glm::vec3 specularColor){
		this->shininess = shininess;
		this->specularStrength = specularStrength;
		this->specularColor = specularColor;
		model = glm::mat4(1);

		loadObj(obj, vertexColor);
	}
	Object::Object(float shininess, float specularStrength, glm::vec3 specularColor){
		this->shininess = shininess;
		this->specularStrength = specularStrength;
		this->specularColor = specularColor;
		model = glm::mat4(1);
	}

	void Object::loadObj(const char *obj, glm::vec3 color){
		// vertex list
		std::vector<glm::vec3> vertices;
		// uv texture list
		std::vector<glm::vec2> uvs;
		// normals list
		std::vector<glm::vec3> normals;
		bool res = loadOBJ(obj, vertices, uvs, normals);
		vetexCount = vertices.size()*3;

		std::vector<glm::vec3> vertexColors;
		for(int i = 0; i < vertices.size(); i++){
			vertexColors.push_back(color);
		}

		loadVAOVBO(vertices, uvs, normals, vertexColors);
	}

	void Object::loadVAOVBO(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals, std::vector<glm::vec3> vertexColors){
		// can't load obj file more than once
		if(isObjLoaded)
			throw "Obj file already loaded for specific object.";
		// std::cout<<"loaded vaovbo from object "<< vertices.size() <<std::endl;

		// VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Move vertex data to video memory; specifically to VBO called vertexbuffer
		glGenBuffers(1, &VBO_vetex);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vetex);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
			
		// Move color data to video memory; specifically to CBO called colorbuffer
		glGenBuffers(1, &VBO_color);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
		glBufferData(GL_ARRAY_BUFFER, vertexColors.size() *sizeof(glm::vec3), &vertexColors[0], GL_STATIC_DRAW);

		// move normal to VRAM
		glGenBuffers(1, &VBO_normals);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glBufferData(GL_ARRAY_BUFFER, normals.size() *sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

		isObjLoaded = true;
	}
	void Object::loadVAOVBO(float *vertices, float *uvs, float *normals, float *vertexColors, int totVertices){
		// can't load obj file more than once
		if(isObjLoaded)
			throw "Obj file already loaded for specific object.";
		// std::cout<<"loaded vaovbo from object "<< totVertices << std::endl;

		vetexCount = totVertices;

		// VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// Move vertex data to video memory; specifically to VBO called vertexbuffer
		glGenBuffers(1, &VBO_vetex);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vetex);
		glBufferData(GL_ARRAY_BUFFER, totVertices * sizeof(float), &vertices[0], GL_STATIC_DRAW);
			
		// Move color data to video memory; specifically to CBO called colorbuffer
		glGenBuffers(1, &VBO_color);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
		glBufferData(GL_ARRAY_BUFFER, totVertices *sizeof(float), &vertexColors[0], GL_STATIC_DRAW);

		// move normal to VRAM
		glGenBuffers(1, &VBO_normals);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glBufferData(GL_ARRAY_BUFFER, totVertices *sizeof(float), &normals[0], GL_STATIC_DRAW);

		isObjLoaded = true;
	}

	void Object::draw(glm::mat4 *projection, glm::mat4 *view, Light *light, glm::vec3 *cameraPosition, GLuint shader){
		// Use our shader
		glUseProgram(shader);
			
		// Send data over to gpu
		glUniformMatrix4fv(glGetUniformLocation(shader, "camera"), 1, GL_FALSE, value_ptr(*projection*(*view)));
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, value_ptr(model));
		// glm::vec3 cp(0,0,0);
		// glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, value_ptr(cp));
		glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, value_ptr(*cameraPosition));
		glUniform1fv(glGetUniformLocation(shader, "materialShininess"), 1, &shininess);
		// std::cout << "specular strength "<< specularStrength << std::endl;
		glUniform1fv(glGetUniformLocation(shader, "specularStrength"), 1, &specularStrength);
		// std::cout << "specular color ("<< specularColor.x << ", " << specularColor.y << ", "<<specularColor.z << ")" << std::endl;
		glUniform3fv(glGetUniformLocation(shader, "materialSpecularColor"), 1, &specularColor[0]);
		// glUniform3fv(glGetUniformLocation(shader, "light.position"), 1, value_ptr(cp));
		glUniform3fv(glGetUniformLocation(shader, "light.position"), 1, value_ptr(light->position));
		// std::cout << "light color ("<< light->color.x << ", " << light->color.y << ", "<<light->color.z << ")" << std::endl;
		glUniform3fv(glGetUniformLocation(shader, "light.intensities"), 1, &light->color[0]);
		glUniform1fv(glGetUniformLocation(shader, "light.attenuation"), 1, &light->intensity);
		// float ac = 1.0f;
		// glUniform1fv(glGetUniformLocation(shader, "light.ambientCoefficient"), 1, &ac);
		glUniform1fv(glGetUniformLocation(shader, "light.ambientCoefficient"), 1, &light->ambientCoefficient);
		glUniform3fv(glGetUniformLocation(shader, "light.ambientColor"), 1, &light->ambientColor[0]);
		
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vetex);
		glVertexAttribPointer(
			0,			// attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
		);
			
		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
		glVertexAttribPointer(
			1,			// attribute. No particular reason for 1, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
		);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glVertexAttribPointer(
			2,			// attribute. No particular reason for 1, but must match the layout in the shader.
			3,			// size
			GL_FLOAT,	// type
			GL_TRUE,	// normalized?
			0,			// stride
			(void*)0	// array buffer offset
		);
			
		glDrawArrays(GL_TRIANGLES, 0, vetexCount);
			
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	Object::~Object(){
		if(!isObjLoaded)
			return;
		std::cout<< "Destructuring Object " << vetexCount<<std::endl;
		glDeleteBuffers(1, &VBO_vetex);
		glDeleteBuffers(1, &VBO_color);
		glDeleteBuffers(1, &VBO_normals);
		glDeleteVertexArrays(1, &VAO);
	}
}