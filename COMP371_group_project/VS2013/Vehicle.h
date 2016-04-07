#pragma once

#include "RawModel.h"
#include <vector>
#include "Street.h"

using namespace std;
using namespace glm;

class Vehicle : public RawModel {
private:
	vector<vec3> positions, indices, normals, colours;
	mat4 modelMatrix;
	mat4 oldModelMatrix;
	Street* current_street;
	Street* last_street;
	vector<Street>* streets;
	bool horizontal;

	vec3 mycolour;

	float currentRotation;
	bool rotating;
	float targetRotation;
	float endRotation;

	const float HOVER = 0.2f;
	float myspeed;
	int mytick;

public:
	void build();
	void bindToModel();
	mat4 getModelMatrix();
	void tick();
	void assignStreet(vector<Street>* street);
};