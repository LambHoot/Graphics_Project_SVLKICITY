#pragma once

#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>

class RawModel
{

private: 
	GLuint vaoID;
	GLuint vertexCount;
	GLuint* indices;

public:
	RawModel(GLuint vaoID, GLuint* indices, GLuint vertexCount);
	~RawModel();

	GLuint getVAOID();
	GLuint* getIndices();
	GLuint getVertexCount();
};

