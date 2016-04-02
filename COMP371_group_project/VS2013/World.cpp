#include "World.h"
#include "Loader.h"

vector<glm::vec3> Worldpositions, Worldindices;

World::World()
{
	World::loadFloor();
	World::bindToModel();
}


World::~World()
{
}

void World::loadFloor(){
	/*
	CImg<unsigned char>  floorImage(128, 128, 1, 3, 0);
	int width = 128, height = 128;
	floorImage.resize(width, height, 1);

	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			floorImage.data[(j*width) + i] = (i < width / 16 || j < height / 16 ? 255 : 0);
		}
	}

	glBindTexture(GL_TEXTURE_2D, floorImage);
	*/
	
	glm::vec3 farLeft = { -500.0f, 0.0f, 500.0f };
	glm::vec3 nearLeft = { -500.0f, 0.0f, -500.0f };
	glm::vec3 farRight = { 500.0f, 0.0f, 500.0f };
	glm::vec3 nearRight = { 500.0f, 0.0f, -500.0f };

	Worldpositions = {
		farLeft,
		nearLeft,
		farRight,
		nearRight };

	Worldindices = {
		glm::vec3(2, 0, 1),
		glm::vec3(1, 2, 3)
	};
}

void World::bindToModel() {
	RawModel temp = Loader::loadToVAO(Worldpositions, Worldindices);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}
