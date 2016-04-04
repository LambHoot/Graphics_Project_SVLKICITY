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
	glBindVertexArray(vaoID);
	loadVertices(positions);
	loadIndices(indices);
	loadColors(colors);
	glBindVertexArray(0);
}

void Building::build(){
	positions = { 
				vec3(-width / 2.0f, 0.0f, width / 2.0f),
				vec3(width / 2.0f, 0.0f, width / 2.0f),
				vec3(width / 2.0f, 0.0f, -width / 2.0f),
				vec3(-width / 2.0f, 0.0f, -width / 2.0f),

				vec3(-width / 2.0f, height, width / 2.0f),
				vec3(width / 2.0f, height, width / 2.0f),
				vec3(width / 2.0f, height, -width / 2.0f),
				vec3(-width / 2.0f, height, -width / 2.0f) };

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