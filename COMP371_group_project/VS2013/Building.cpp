#include "Building.h"
#include "Loader.h"

//remove as not needed, just copied everything used in main
using namespace std;

//Constructor
Building::Building(float h, float w)
{
	this->height = h;
	this->width = w;

	Building::build();
}
//not sure what this is
Building::~Building()
{
}

void Building::build(){
	GLfloat positions[] = { -width / 2.0f, 0.0f, width / 2.0f,
							width / 2.0f, 0.0f, width / 2.0f,
							width / 2.0f, 0.0f, -width / 2.0f,
							-width / 2.0f, 0.0f, -width / 2.0f,

							-width / 2.0f, height, width / 2.0f,
							width / 2.0f, height, width / 2.0f,
							width / 2.0f, height, -width / 2.0f,
							-width / 2.0f, height, -width / 2.0f };

	//TODO: Think of efficient indexing algorithm. Hopefully in tandem with vertex placement
	GLuint indices[] = {	0, 1, 2,
							0, 2, 3,
							4, 5, 6,
							4, 6, 7,
							0, 1, 5,
							0, 5, 4,
							1, 2, 6,
							1, 6, 5,
							2, 3, 7,
							2, 7, 6,
							3, 0, 4,
							3, 4, 7	};

	RawModel temp = Loader::loadToVAO(positions, sizeof(positions)/sizeof(*positions), indices, sizeof(indices)/sizeof(*indices));
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}