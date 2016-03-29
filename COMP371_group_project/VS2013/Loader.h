#include <iostream>

using namespace std;

#include "RawModel.h"

static class Loader
{
private:
	static vector<GLuint> VAO, VBO;

	static void storeDataInAttribList(int attNumber, GLfloat list[], int data_size);
	static void bindIndicesBuffer(GLuint indices[], int data_size);

public:
	Loader();
	~Loader();

	static RawModel loadToVAO(GLfloat positions[], int positions_length, GLuint indices[], int indices_length);
	static bool cleanUp();
};

