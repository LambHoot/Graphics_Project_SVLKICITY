#pragma once


#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>
#include "CImg.h"

#include "RawModel.h"

class World : public RawModel
{
private:
	vec3 color = {0.3f, 0.6f, 0.3f};

public:
	World(glm::vec3 fl, glm::vec3 br);
	~World();
	void loadFloor(glm::vec3 fl, glm::vec3 br);
	void bindToModel();
};

