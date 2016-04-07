#include "Loader.h"
vector<GLuint> Loader::VAO;
vector<GLuint> Loader::VBO;


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

RawModel Loader::loadToVAO(vector <glm::vec3> positions, vector <glm::vec3> normals, vector<glm::vec3> indices, vector<glm::vec3> colors) {
	GLfloat *pos = new GLfloat[positions.size() * 3];
	GLuint *ind = new GLuint[indices.size() * 3];
	GLfloat *col = new GLfloat[colors.size() * 3];
	GLfloat *nor = new GLfloat[normals.size() * 3];

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

	for (unsigned int j = 0; j < normals.size(); j++){
		nor[3 * j + 0] = normals[j].x;
		nor[3 * j + 1] = normals[j].y;
		nor[3 * j + 2] = normals[j].z;
	}

	for (unsigned int j = 0; j < colors.size(); j++){
		col[3 * j + 0] = colors[j].x;
		col[3 * j + 1] = colors[j].y;
		col[3 * j + 2] = colors[j].z;
	}

	return loadToVAO(pos, positions.size() * point_size, nor, normals.size() * point_size, ind, indices.size() * point_size, col, colors.size()*point_size);
}

RawModel Loader::loadToVAO(GLfloat positions[], int positions_length, GLfloat normals[], int normals_length, GLuint indices[], int indices_length, GLfloat colors[], int colors_length){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	bindIndicesBuffer(indices, sizeof(indices)*indices_length);
	storeDataInAttribList(0, positions, sizeof(positions)*positions_length);
	storeDataInAttribList(1, normals, sizeof(normals)*normals_length);
	storeDataInAttribList(2, colors, sizeof(colors)*colors_length);
	glBindVertexArray(0);

	VAO.push_back(vao);

	return RawModel(vao, indices_length);
}

bool Loader::cleanUp() {
	glDisableVertexAttribArray(0);
	//Properly de-allocate all resources once they've outlived their purpose

	GLuint *vao, *vbo;

	cout << "Deleting VAOs..." << endl;
	for (unsigned int i = 0; i < VAO.size(); i++){
		vao = &VAO[i];
		//cout << "Deleting VAO id = " << vao << endl;
		glDeleteVertexArrays(1, vao);
	}

	cout << "Deleting VBOs..." << endl;
	for (unsigned int i = 0; i < VBO.size(); i++){
		vbo = &VBO[i];
		//cout << "Deleting VBO id = " << vbo << endl;
		glDeleteBuffers(1, vbo);
	}

	// Close GL context and any other GLFW resources
	glfwTerminate();
	return true;
}

vector < glm::vec3> Loader::generateNormals(vector<glm::vec3> vertices, vector<glm::vec3> indices){
	vector<glm::vec3> normals;
	vector<glm::vec3> shared;
	vector<glm::vec3> triangleNormals;

	// generate triangle normals in same order as indices
	for (unsigned int x = 0; x < indices.size(); x++) {
		glm::vec3 e1 = vertices[indices[x].y] - vertices[indices[x].x];
		glm::vec3 e2 = vertices[indices[x].z] - vertices[indices[x].x];

		triangleNormals.push_back(glm::normalize(glm::cross(e1, e2)));
	}

	//For every vertex
	for (unsigned int i = 0; i < vertices.size(); i++){
		shared.clear();
		glm::vec3 norm = { 0, 0, 0 };

		// Find all Shared Normals
		for (unsigned int j = 0; j < indices.size(); j++) {
			if (indices[j].x == i || indices[j].y == i || indices[j].z == i){
				bool found = false;

				for (unsigned int k = 0; k < shared.size(); k++)
				{
					if (shared[k] == triangleNormals[j])
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					shared.push_back(triangleNormals[j]);
				}
			}
		}

		// Add all shared normals
		for (glm::vec3 s : shared){
			norm += s;
		}

		norm / (float)shared.size();

		glm::vec3 normed = glm::normalize(norm);
		cout << "glm::vec3(" << normed.x << ", " << normed.y << ", " << normed.z << ")" << endl;
		normals.push_back(glm::normalize(norm));
	}

	system("pause");

	return normals;
}
