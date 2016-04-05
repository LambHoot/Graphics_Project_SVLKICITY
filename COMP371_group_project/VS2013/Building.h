#pragma once


#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>
#include <stdlib.h>

using namespace std;

#include "RawModel.h"

class Building : public RawModel
{

private:

	void build();
	void sendToPosition();
	void bindToModel();

public:
	float height;
	float width;
	float depth;
	glm::vec3 position;
	Building(float h, float w, float d, glm::vec3 position);
	Building(float h, float w, glm::vec3 position);
	Building(float h, float w);
	static Building generateRandomBuilding(glm::vec3 position, float max, glm::vec2 block);
	static bool checkIfConflict(Building build, vector<Building> buildList, float s1, float s2, float xOff, float zOff);
	bool Building::isBuildingPointLegal(glm::vec3 point);
	~Building();

};

