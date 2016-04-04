#include "Building.h"
#include "Loader.h"

//remove as not needed, just copied everything used in main
using namespace std;
using namespace glm;

vector<vec3> positions, indices, colors;
//Constructors
Building::Building(float h, float w) : Building(h, w, vec3(0,0,0))
{
}

Building::Building(float h, float w, vec3 position) : RawModel()
{
}

Building::Building(float h, float w, float d, glm::vec3 position){
	this->height = h;
	this->width = w;
	this->depth = d;
	this->position = position;

	Building::build();
	Building::sendToPosition();
	Building::bindToModel();
}

//not sure what this is
Building::~Building()
{
}

void Building::bindToModel() {
	glBindVertexArray(vaoID);
	loadVertices(positions);
	loadIndices(indices);
	loadColors(colors);
	glBindVertexArray(0);
}

void Building::build(){
	positions = { 
				glm::vec3(-width / 2.0f, 0.0f, depth / 2.0f),
				glm::vec3(width / 2.0f, 0.0f, depth / 2.0f),
				glm::vec3(width / 2.0f, 0.0f, -depth / 2.0f),
				glm::vec3(-width / 2.0f, 0.0f, -depth / 2.0f),

				glm::vec3(-width / 2.0f, height, depth / 2.0f),
				glm::vec3(width / 2.0f, height, depth / 2.0f),
				glm::vec3(width / 2.0f, height, -depth / 2.0f),
				glm::vec3(-width / 2.0f, height, -depth / 2.0f) };

	//TODO: Think of efficient indexing algorithm. Hopefully in tandem with vertex placement
	 indices = {	
				vec3(0, 1, 2),
				vec3(0, 2, 3),
				vec3(4, 5, 6),
				vec3(4, 6, 7),
				vec3(0, 1, 5),
				vec3(0, 5, 4),
				vec3(1, 2, 6),
				vec3(1, 6, 5),
				vec3(2, 3, 7),
				vec3(2, 7, 6),
				vec3(3, 0, 4),
				vec3(3, 4, 7) };

	 //temporary color randomizer
	 for (unsigned int i = 0; i < positions.size(); i++){
		 colors.push_back(vec3(
			 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			 static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			 static_cast <float> (rand()) / static_cast <float> (RAND_MAX))
			 );
	 }
}

void Building::sendToPosition(){
	for (unsigned int i = 0; i < positions.size(); i++) {
		positions[i].x += position.x;
		positions[i].y += position.y;
		positions[i].z += position.z;
	}
	
}

Building Building::generateRandomBuilding(glm::vec3 position, float max){
	float lowSize = max/20.0f;
	float highSize = max/5.0f;
	float width = lowSize + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highSize - lowSize)));
	float depth = lowSize + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highSize - lowSize)));
	float heightFactor = glm::length((position)/50.0f);
	heightFactor = (1.0f / heightFactor)*2.0f;
	float lowHeight = max/10.0f;
	float highHeight = (max / 5.0f)*heightFactor;
	float height = (lowHeight + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highHeight - lowHeight))));
	Building bb = Building(height, width, depth, position);
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
		//if (((topZb >= bottomZ) && ((topZb <= topZ))) && ((leftXb >= leftX) && (leftXb <= rightX))){
		//	return false;
		//}
		//else if ((topZb >= bottomZ) && ((topZb <= topZ)) && ((rightXb >= leftX) && (rightXb <= rightX))){
		//	return false;
		//}
		//else if (((bottomZb >= bottomZ) && ((bottomZb <= topZ))) && ((leftXb >= leftX) && (leftXb <= rightX))){
		//	return false;
		//}
		//else if ((bottomZb >= bottomZ) && ((bottomZb <= topZ)) && ((rightXb >= leftX) && (rightXb <= rightX))){
		//	return false;
		//}

		if ((topZb > bottomZ) && (topZb < topZ)){
			if ((leftXb > leftX) && (leftXb < rightX)){
				return false;
			}
		}
		else if ((topZb > bottomZ) && (topZb < topZ)){
			if ((rightXb > leftX) && (rightXb < rightX)){
				return false;
			}
		}
		if ((bottomZb > bottomZ) && ((bottomZb < topZ))){
			if ((leftXb > leftX) && (leftXb < rightX)){
				return false;
			}
		}
		if ((bottomZb > bottomZ) && ((bottomZb < topZ))){
			if ((rightXb > leftX) && (rightXb < rightX)){
				return false;
			}
		}
		
	}
	return true;
}