#pragma once

#include "RawModel.h"
#include <vector>
#include "Street.h"

using namespace std;
using namespace glm;

class Vehicle : public RawModel {
private:
	vector<vec3> positions, indices;
	mat4 modelMatrix;
	Street* current_street;
	Street* last_street;
	vector<Street>* streets;
	bool horizontal;
public:
	void build();
	void bindToModel();
	mat4 getModelMatrix();
	void tick();
	void assignStreet(vector<Street>* street);
};