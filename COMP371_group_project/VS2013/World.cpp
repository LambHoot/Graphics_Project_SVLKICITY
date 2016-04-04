#include "World.h"
#include "Loader.h"

using namespace glm;

vector<vec3> Worldpositions, Worldindices;

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
		vec3(2, 0, 1),
		vec3(1, 2, 3)
	};
}

void World::bindToModel() {
	RawModel temp = Loader::loadToVAO(Worldpositions, Worldindices);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}

bool World::isPointLegal(glm::vec3 point) {
	return point.y > TOP_PADDING;
}