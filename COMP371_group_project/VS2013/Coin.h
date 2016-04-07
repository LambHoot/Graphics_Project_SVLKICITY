#pragma once

#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>
#include "CImg.h"
#include <gtx/rotate_vector.hpp>

using namespace cimg_library;
using namespace std;
using namespace glm;

#include "RawModel.h"

class Coin : public RawModel
{

private:

public:
	mat4* coinModel;
	vec3 mycoinPosition;

	vector<vec3> CoinPositions, CoinIndices;
	glm::vec3 position;
	Coin(glm::vec3 pos);
	~Coin();
	void bindToModel();
	static bool isCoinTouched(Coin c, vec3 point);
	static void rotateToFace(Coin c, vec3 point, vec3 camRight);
};

