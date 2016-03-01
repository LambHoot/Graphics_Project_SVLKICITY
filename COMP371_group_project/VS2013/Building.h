#pragma once


#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>

using namespace std;


class Building
{

private:
	float height;
	float width;
	glm::vec3 center;
	vector<glm::vec3> bldgPoints;


public:
	Building(float h, float w, glm::vec3 c);
	~Building();

	vector<glm::vec3> generateBldg(float h, float w, glm::vec3 c);
	vector<glm::vec3> generateRandomBldg(float wRangeLO, float wRangeHI, float hRangeLO, float hRangeHI, glm::vec3 c);
	vector<glm::vec3> getBldgPoints();
};

