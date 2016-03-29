#include "Building.h"
#include "Loader.h"

//remove as not needed, just copied everything used in main
using namespace std;

vector<glm::vec3> positions, indices;
//Constructors
Building::Building(float h, float w) : Building(h, w, glm::vec3(0,0,0))
{
}

Building::Building(float h, float w, glm::vec3 position)
{
	this->height = h;
	this->width = w;
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
				glm::vec3(-width / 2.0f, 0.0f, width / 2.0f),
				glm::vec3(width / 2.0f, 0.0f, width / 2.0f),
				glm::vec3(width / 2.0f, 0.0f, -width / 2.0f),
				glm::vec3(-width / 2.0f, 0.0f, -width / 2.0f),

				glm::vec3(-width / 2.0f, height, width / 2.0f),
				glm::vec3(width / 2.0f, height, width / 2.0f),
				glm::vec3(width / 2.0f, height, -width / 2.0f),
				glm::vec3(-width / 2.0f, height, -width / 2.0f) };

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
	//TODO translate a point by another point
}