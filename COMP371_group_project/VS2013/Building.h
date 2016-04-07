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
using namespace glm;

#include "RawModel.h"

using namespace glm;

class Building : public RawModel
{

private:
	const float SIDE_COLLISION_PADDING = 0.2f;
	const float TOP_COLLISION_PADDING = 0.5f;

	void build();
	void build(vec3 passColor);
	void sendToPosition();
	void bindToModel();

public:
	float height;
	float width;
	float depth;
	vector<glm::vec3> colors;
	vector<glm::vec3> positions;
	glm::vec3 position;
	Building(float h, float w, float d, glm::vec3 position);
	Building(float h, float w, float d, glm::vec3 position, vec3 passedColor);
	Building(float h, float w, glm::vec3 position);
	Building(float h, float w);
	static Building* generateRandomBuilding(glm::vec3 position, float max, glm::vec2 block);
	static bool checkIfConflict(Building build, vector<Building> buildList, float s1, float s2, float xOff, float zOff);
	bool Building::isBuildingPointLegal(glm::vec3 point);
	~Building();

	bool isPointLegal(vec3 point) override;
};

