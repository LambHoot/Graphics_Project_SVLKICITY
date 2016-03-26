#include "Building.h"


//remove as not needed, just copied everything used in main
using namespace std;

//Constructor
Building::Building(float h, float w, glm::vec3 c)
{
	height = h;
	width = w;
	center = c;
	bldgPoints = {};
}
//not sure what this is
Building::~Building()
{
}

vector<glm::vec3> Building::getBldgPoints() {
	return bldgPoints;
}

vector<glm::vec3> Building::generateBldg(float h, float w, glm::vec3 c) {
	Building bldg = Building(h, w, c);
	//pick some 3D model from some directory of building models
	//scale it to meet the width and height defined
	//pushback all of its points to bldgPoints for this building
	return getBldgPoints();
}

vector<glm::vec3> Building::generateRandomBldg(float wRangeLO, float wRangeHI, float hRangeLO, float hRangeHI, glm::vec3 c) {
	//from http://stackoverflow.com/questions/686353/c-random-float-number-generation
	float w = wRangeLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (wRangeHI - wRangeLO)));
	float h = hRangeLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (hRangeHI - hRangeLO)));
	return generateBldg(w,h,c);
}
