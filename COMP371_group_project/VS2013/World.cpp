#include "World.h"
#include "Loader.h"

vector<glm::vec3> Worldpositions, Worldindices, Worldcolors;

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

	Worldindices = {
		glm::vec3(2, 0, 1),
		glm::vec3(1, 2, 3)
	};

	for (unsigned int i = 0; i < Worldpositions.size(); i++){
		Worldcolors.push_back(glm::vec3(
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX))
			);
	}
	
}

void World::bindToModel() {
	RawModel temp = Loader::loadToVAO(Worldpositions, Worldindices, Worldcolors);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}
