#include "Building.h"
#include "Loader.h"

//remove as not needed, just copied everything used in main
using namespace std;

vector<glm::vec3> positions, indices;
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
	Building::sendToPosition();
	Building::bindToModel();
}

//not sure what this is
Building::~Building()
{
}

void Building::bindToModel() {
	RawModel temp = Loader::loadToVAO(positions, indices);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
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

bool Building::checkIfConflict(Building build, vector<Building> buildList){
	for (int bl = 0; bl < buildList.size(); bl++){
		glm::vec3 b1tl = { build.position.x - build.width / 2.0f, build.position.y, build.position.z + build.depth / 2.0f };
		glm::vec3 b1br = { build.position.x + build.width / 2.0f, build.position.y, build.position.z - build.depth / 2.0f };
		glm::vec3 b2tl = { buildList[bl].position.x - buildList[bl].width / 2.0f, buildList[bl].position.y, buildList[bl].position.z + buildList[bl].depth / 2.0f };
		glm::vec3 b2br = { buildList[bl].position.x + buildList[bl].width / 2.0f, buildList[bl].position.y, buildList[bl].position.z - buildList[bl].depth / 2.0f };
		if (((b1tl.x <= b2tl.x) && (b2tl.x <= b1br.x)) && ((b1tl.z <= b2tl.z) && (b2tl.z <= b1br.z))){
			return false;
		}
	}
	return true;
}