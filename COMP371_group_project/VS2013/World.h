#pragma once


#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

#include "RawModel.h"

class World : public RawModel
{
public:
	World();
	~World();
	void loadFloor();
	void bindToModel();
};

