#include "World.h"
#include "Loader.h"

using namespace glm;

vector<vec3> Worldpositions, Worldindices, Worldcolors;

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

	Worldcolors = {
		color, color, color, color
	};
}

void World::bindToModel() {
	glBindVertexArray(vaoID);
	loadVertices(Worldpositions);
	loadIndices(Worldindices);
	loadColors(Worldcolors);
	glBindVertexArray(0);
}
