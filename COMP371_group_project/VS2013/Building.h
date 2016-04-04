#pragma once


#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <vector>

using namespace std;
using namespace glm;

#include "RawModel.h"

using namespace glm;

class Building : public RawModel
{

private:
	const float SIDE_COLLISION_PADDING = 0.2f;
	const float TOP_COLLISION_PADDING = 0.5f;


	float height;
	float width;
	vec3 position;

	void build();
	void sendToPosition();
	void bindToModel();

public:
	Building(float h, float w, vec3 position);
	Building(float h, float w);
	~Building();

	bool isPointLegal(vec3 point) override;
};

