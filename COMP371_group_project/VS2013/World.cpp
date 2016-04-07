#include "World.h"
#include "Loader.h"

vector<glm::vec3> Worldpositions, Worldindices, Worldcolors, Worldnormals;

World::World(glm::vec3 fl, glm::vec3 br)
{
	World::loadFloor(fl, br);
	World::bindToModel();
}


World::~World()
{
}

void World::loadFloor(glm::vec3 fl, glm::vec3 br){
	glm::vec3 farLeft = { fl.x, 0.0f, fl.z };
	glm::vec3 nearLeft = { fl.x, 0.0f, br.z };
	glm::vec3 farRight = { br.x, 0.0f, fl.z };
	glm::vec3 nearRight = { br.x, 0.0f, br.z };

	Worldpositions = {
		farLeft,
		nearLeft,
		farRight,
		nearRight };

	Worldnormals = {
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0)
	};

	Worldindices = {
		glm::vec3(2, 0, 1),
		glm::vec3(1, 2, 3)
	};

	for (unsigned int i = 0; i < Worldpositions.size(); i++){
		GLfloat monochrome = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Worldcolors.push_back(glm::vec3(
			0.5,
			0.5,
			0.5)
			);
	}
	
}

void World::bindToModel() {
	RawModel temp = Loader::loadToVAO(Worldpositions, Worldnormals, Worldindices, Worldcolors);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}

bool World::isPointLegal(glm::vec3 point) {
	return point.y > TOP_PADDING;
}