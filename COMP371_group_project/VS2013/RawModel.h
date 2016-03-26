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


public:
	RawModel(GLuint vaoID, GLuint vertexCount);
	~RawModel();

	GLuint getVAOID();
	GLuint getVertexCount();
};

