#include <iostream>
#include "glm.hpp"

using namespace std;

#include "RawModel.h"

static class Loader
{
private:
	static vector<GLuint> VAO, VBO;

	static void storeDataInAttribList(int attNumber, GLfloat list[], int data_size);
	static void bindIndicesBuffer(GLuint indices[], int data_size);
	static RawModel loadToVAO(GLfloat positions[], int positions_length, GLuint indices[], int indices_length);
public:
	Loader();
	~Loader();

	static RawModel loadToVAO(vector<glm::vec3> positions, vector<glm::vec3> indices);
	static bool cleanUp();
};

