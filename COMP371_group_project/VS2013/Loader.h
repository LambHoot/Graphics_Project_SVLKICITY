#include <iostream>
#include "glm.hpp"

using namespace std;
using namespace glm;

#include "RawModel.h"

static class Loader
{
private:
	static vector<GLuint> VAO, VBO;
	static const int point_size = 3;
public:
	Loader();
	~Loader();

	static GLuint createNewVAO();
	static GLuint createNewVBO();
	static void storeDataInAttribList(int attNumber, GLfloat list[], int data_size);
	
	static void bindIndicesBuffer(GLuint indices[], int data_size);

	static bool cleanUp();
};

