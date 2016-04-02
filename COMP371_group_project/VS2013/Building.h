#pragma once


#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>

using namespace std;

#include "RawModel.h"

class Building : public RawModel
{

private:
	float height;
	float width;
	glm::vec3 position;

	void build();
	void sendToPosition();
	void bindToModel();

public:
	Building(float h, float w, glm::vec3 position);
	Building(float h, float w);
	~Building();

};

