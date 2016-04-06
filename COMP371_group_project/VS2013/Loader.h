#include <iostream>
#include "glm.hpp"

using namespace std;

#include "RawModel.h"

static class Loader
{
private:
	static vector<GLuint> VAO, VBO;
	static const int point_size = 3;

	static RawModel loadToVAO(GLfloat positions[], int positions_length, GLfloat normals[], int normals_length, GLuint indices[], int indices_length, GLfloat colors[], int colors_length);
	static void storeDataInAttribList(int attNumber, GLfloat list[], int data_size);
	static void bindIndicesBuffer(GLuint indices[], int data_size);
	static vector < glm::vec3> generateNormals(vector<glm::vec3> vertices, vector<glm::vec3> indices);
	
public:
	Loader();
	~Loader();

	static RawModel loadToVAO(vector<glm::vec3> positions, vector<glm::vec3> normals, vector<glm::vec3> indices, vector<glm::vec3> colors);
	static bool cleanUp();
	static void updateVertices(GLuint vaoID, vector<glm::vec3> positions);
};

