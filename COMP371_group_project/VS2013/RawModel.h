#pragma once

#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>

using namespace std;
using namespace glm;

class RawModel
{

protected: 

	static const int point_size = 3;
	GLuint vaoID;
	GLuint elementCount;

public:
	RawModel();
	RawModel(GLuint vaoID, GLuint elementCount);
	~RawModel();

	GLuint getVAOID();
	GLuint getelementCount();

	void loadVertices(vector<vec3> vertices);
	void loadIndices(vector<vec3> indices);
};

