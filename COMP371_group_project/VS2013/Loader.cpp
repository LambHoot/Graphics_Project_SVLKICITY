#include "Loader.h"
vector<GLuint> Loader::VAO;
vector<GLuint> Loader::VBO;

using namespace glm;

Loader::Loader()
{
}


Loader::~Loader()
{
}

GLuint Loader::createNewVAO(){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	VAO.push_back(vao);

	return vao;
}

GLuint Loader::createNewVBO(){
	GLuint vbo;
	glGenBuffers(1, &vbo);
	VBO.push_back(vbo);

	return vbo;
}

void Loader::bindIndicesBuffer(GLuint indices[], int data_size){
	GLuint vbo = createNewVBO();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, indices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::storeDataInAttribList(int attNumber, GLfloat list[], int data_size){
	GLuint vbo = createNewVBO();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data_size, list, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(attNumber);
	glVertexAttribPointer(
		attNumber,
		3,                  // 3 for 3D
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // Distance between vertices in array
		(void*)0            // array buffer offset
		);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Loader::cleanUp() {
	glDisableVertexAttribArray(0);
	//Properly de-allocate all resources once they've outlived their purpose

	GLuint *vao, *vbo;
	for (unsigned int i = 0; i < VAO.size(); i++){
		vao = &VAO[i];
		cout << "Deleting VAO id = " << vao << endl;
		glDeleteVertexArrays(1, vao);
	}
	for (unsigned int i = 0; i < VBO.size(); i++){
		vbo = &VBO[i];
		cout << "Deleting VBO id = " << vbo << endl;
		glDeleteBuffers(1, vbo);
	}

	// Close GL context and any other GLFW resources
	glfwTerminate();
	return true;
}
