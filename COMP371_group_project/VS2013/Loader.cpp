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

void Loader::bindIndicesBuffer(GLuint indices[], int data_size){
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, indices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);// added by phil -- to be seen if any trouble caused

	VBO.push_back(vbo);
}

void Loader::storeDataInAttribList(int attNumber, GLfloat list[], int data_size){
	GLuint vbo;
	glGenBuffers(1, &vbo);
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

	VBO.push_back(vbo);
}

RawModel Loader::loadToVAO(vector <vec3> positions, vector<vec3> indices) {
	GLfloat *pos = new GLfloat[positions.size() * 3];
	GLuint *ind = new GLuint[indices.size() * 3];

	for (unsigned int i = 0; i < positions.size(); i++){
		pos[3 * i + 0] = positions[i].x;
		pos[3 * i + 1] = positions[i].y;
		pos[3 * i + 2] = positions[i].z;
	}

	for (unsigned int j = 0; j < indices.size(); j++){
		ind[3 * j + 0] = indices[j].x;
		ind[3 * j + 1] = indices[j].y;
		ind[3 * j + 2] = indices[j].z;
	}

	return loadToVAO(pos, positions.size() * point_size, ind, indices.size() * point_size);
}

RawModel Loader::loadToVAO(GLfloat positions[], int positions_length, GLuint indices[], int indices_length){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	bindIndicesBuffer(indices, sizeof(indices)*indices_length);
	storeDataInAttribList(0, positions, sizeof(positions)*positions_length);
	glBindVertexArray(0);

	VAO.push_back(vao);

	return RawModel(vao, indices_length);
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
