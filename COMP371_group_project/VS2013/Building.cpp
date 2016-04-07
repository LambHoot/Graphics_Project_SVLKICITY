#include "Building.h"
#include "Loader.h"
#include "Street.h"

//remove as not needed, just copied everything used in main
using namespace std;
using namespace glm;

vector<glm::vec3> positions, indices, colors, normals;
//Constructors


Building::Building(float h, float w) : Building(h, w, w, glm::vec3(0,0,0))
{
}

Building::Building(float h, float w, glm::vec3 position) : Building(h, w, w, position)
{
}

Building::Building(float h, float w, float d, glm::vec3 position){
	this->height = h;
	this->width = w;
	this->depth = d;
	this->position = position;

	Building::build();
	Building::bindToModel();
}

//not sure what this is
Building::~Building()
{
}

void Building::bindToModel() {
	RawModel temp = Loader::loadToVAO(positions, normals, indices, colors);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}

void Building::build(){
	positions = { 
				position + glm::vec3(-width / 2.0f, 0.0f, depth / 2.0f),
				position + glm::vec3(width / 2.0f, 0.0f, depth / 2.0f),
				position + glm::vec3(width / 2.0f, 0.0f, -depth / 2.0f),
				position + glm::vec3(-width / 2.0f, 0.0f, -depth / 2.0f),

				position + glm::vec3(-width / 2.0f, height, depth / 2.0f),
				position + glm::vec3(width / 2.0f, height, depth / 2.0f),
				position + glm::vec3(width / 2.0f, height, -depth / 2.0f),
				position + glm::vec3(-width / 2.0f, height, -depth / 2.0f) };

	normals = {
		glm::vec3(-0.57735, 0.57735, 0.57735),
		glm::vec3(0.57735, 0.57735, 0.57735),
		glm::vec3(0.57735, 0.57735, -0.57735),
		glm::vec3(-0.57735, 0.57735, -0.57735),
		glm::vec3(-0.57735, 0.57735, 0.57735),
		glm::vec3(0.57735, 0.57735, 0.57735),
		glm::vec3(0.57735, 0.57735, -0.57735),
		glm::vec3(-0.57735, 0.57735, -0.57735)
	};
	

	//TODO: Think of efficient indexing algorithm. Hopefully in tandem with vertex placement
	 indices = {	
				glm::vec3(0, 1, 2),
				glm::vec3(0, 2, 3),
				glm::vec3(4, 5, 6),
				glm::vec3(4, 6, 7),
				glm::vec3(0, 1, 5),
				glm::vec3(0, 5, 4),
				glm::vec3(1, 2, 6),
				glm::vec3(1, 6, 5),
				glm::vec3(2, 3, 7),
				glm::vec3(2, 7, 6),
				glm::vec3(3, 0, 4),
				glm::vec3(3, 4, 7) };

	 
	 colors = {
		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),

		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX)),
		 glm::vec3(
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		 static_cast <float> (rand()) / static_cast <float> (RAND_MAX))
	 };
	 
}

void Building::sendToPosition(){
	for (unsigned int i = 0; i < positions.size(); i++) {
		positions[i].x += position.x;
		positions[i].y += position.y;
		positions[i].z += position.z;
	}
	
}

bool Building::isPointLegal(vec3 point) {

	if (point.x > position.x - width / 2.0f - SIDE_COLLISION_PADDING && point.x < position.x + width / 2.0f + SIDE_COLLISION_PADDING &&
		point.y > position.y - SIDE_COLLISION_PADDING && point.y < position.y + height + TOP_COLLISION_PADDING &&
		point.z > position.z - depth / 2.0f  - SIDE_COLLISION_PADDING && point.z < position.z + depth / 2.0f + SIDE_COLLISION_PADDING)
	{
		return false;
	}

	return true;
}

bool Building::isBuildingPointLegal(glm::vec3 point) {
	//bool inBuilding = false;
	float SIDE_COLLISION_PADDING = 1.0f;
	if (point.x > position.x - width / 2.0f - SIDE_COLLISION_PADDING && point.x < position.x + width / 2.0f + SIDE_COLLISION_PADDING &&
		point.y > position.y - SIDE_COLLISION_PADDING && point.y < position.y + height &&
		point.z > position.z - width / 2.0f - SIDE_COLLISION_PADDING && point.z < position.z + width / 2.0f + SIDE_COLLISION_PADDING)
	{
		return false;
	}

	return true;
}

Building* Building::generateRandomBuilding(glm::vec3 position, float max, glm::vec2 block, float heightBoost){
	float lowSize = max/20.0f;
	float highSize = max/5.0f;
	float width = lowSize + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highSize - lowSize)));
	float depth = lowSize + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highSize - lowSize)));
	float height = 0;

	//float heightFactor = glm::length((position)/50.0f);
	float heightFactor = ((abs(block[0]) + abs(block[1])/2.0f)) * heightBoost;
	float lowHeight = (max / 2.0f)*heightFactor;
	float highHeight = (max / 5.0f)*heightFactor;

	if ((abs(block[0]) + abs(block[1]) > ((max / 10) - 4.0f))){
		lowHeight = 15.0f;
		highHeight = 30.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	else if ((abs(block[0]) + abs(block[1]) > ((max / 10) - 6.0f))){
		lowHeight = 15.0f;
		highHeight = 40.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	else if ((abs(block[0]) + abs(block[1]) > ((max / 10) - 7.0f))){
		lowHeight = 20.0f;
		highHeight = 50.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	else if ((abs(block[0]) + abs(block[1]) > ((max / 10) - 8.0f))){
		lowHeight = 25.0f;
		highHeight = 60.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	else if ((abs(block[0]) + abs(block[1]) > ((max / 10) - 8.5f))){
		lowHeight = 30.0f;
		highHeight = 70.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	else if ((abs(block[0]) + abs(block[1]) > ((max / 10) - 9.25f))){
		lowHeight = 35.0f;
		highHeight = 80.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	else if ((abs(block[0]) + abs(block[1]) > ((max / 10) - 9.9f))){
		lowHeight = 40.0f;
		highHeight = 90.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	else{
		height = 30.0f;
		height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	}
	Building* bb = new Building(height, width, depth, position);
	return bb;
}

bool Building::checkIfConflict(Building build, vector<Building> buildList, float sx, float sz, float xOff, float zOff){

	float topZb = build.position.z + (build.depth) / 2.0f;
	float bottomZb = build.position.z - (build.depth) / 2.0f;
	float leftXb = build.position.x - (build.width) / 2.0f;
	float rightXb = build.position.x + (build.width) / 2.0f;

	float topZ = sz + 9.0f * zOff;
	float bottomZ = sz;
	float leftX = sx;
	float rightX = sx + 9.0f * xOff;

	//check streets
	if (topZb > topZ){
		return false;
	}
	else if (bottomZb < bottomZ){
		return false;
	}
	else if (leftXb < leftX){
		return false;
	}
	else if (rightXb > rightX){
		return false;
	}

	//check buildings
	for (int bi = 0; bi < buildList.size(); bi++){
		topZ = buildList[bi].position.z + (buildList[bi].depth) / 2.0f;
		bottomZ = buildList[bi].position.z - (buildList[bi].depth) / 2.0f;
		leftX = buildList[bi].position.x - (buildList[bi].width) / 2.0f;
		rightX = buildList[bi].position.x + (buildList[bi].width) / 2.0f;

		glm::vec3 bTL = { leftXb, 0.0f, topZb };
		glm::vec3 bTR = { rightXb, 0.0f, topZb };
		glm::vec3 bBL = { leftXb, 0.0f, bottomZb };
		glm::vec3 bBR = { rightXb, 0.0f, bottomZb };

		glm::vec3 TL = { leftX, 0.0f, topZ };
		glm::vec3 TR = { rightX, 0.0f, topZ };
		glm::vec3 BL = { leftX, 0.0f, bottomZ };
		glm::vec3 BR = { rightX, 0.0f, bottomZ };

		if ((!(buildList[bi].isBuildingPointLegal(bTL))) || (!(build.isBuildingPointLegal(TL))))
			return false;
		if ((!(buildList[bi].isBuildingPointLegal(bTR))) || (!(build.isBuildingPointLegal(TR))))
			return false;
		if ((!(buildList[bi].isBuildingPointLegal(bBL))) || (!(build.isBuildingPointLegal(BL))))
			return false;
		if ((!(buildList[bi].isBuildingPointLegal(bBR))) || (!(build.isBuildingPointLegal(BR))))
			return false;
	}
	return true;
}