#include "Street.h"
#include "Loader.h"

vector<glm::vec3> Streetpositions, Streetindices, Streetcolors, Streetnormals;

Street::Street(glm::vec3 topLeft, glm::vec3 bottomRight)
{
	Street::loadRoad(topLeft, bottomRight);
	Street::bindToModel();

	this->topLeft = topLeft;
	this->bottomRight = bottomRight;
}


Street::~Street()
{
}

void Street::loadRoad(glm::vec3 topLeft, glm::vec3 bottomRight){
	float yVal = 0.1f;
	Streetpositions = {
		glm::vec3{ topLeft.x, yVal, topLeft.z },
		glm::vec3{ topLeft.x, yVal, bottomRight.z },
		glm::vec3{ bottomRight.x, yVal, topLeft.z },
		glm::vec3{ bottomRight.x, yVal, bottomRight.z }
	};

	Streetindices = {
		glm::vec3(3, 0, 1),
		glm::vec3(3, 2, 0)
	};

	for (unsigned int i = 0; i < Streetpositions.size(); i++){
		GLfloat monochrome = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Streetcolors.push_back(glm::vec3(
			0,
			0,
			0)
			);
	}

	Streetnormals = {
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0)
	};
}

void Street::bindToModel() {
	RawModel temp = Loader::loadToVAO(Streetpositions, Streetnormals, Streetindices, Streetcolors);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}

bool Street::isPointLegal(vec3 point) {
	return true;
}