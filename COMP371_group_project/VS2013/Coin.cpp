#include "Coin.h"
#include "Loader.h"

//remove as not needed, just copied everything used in main
using namespace std;
using namespace glm;

vector<vec3> CoinPositions, CoinIndices;
//Constructors

Coin::Coin(glm::vec3 pos)
{
	//creates a coin (2D square 4.0fx4.0f) at this position
	glm::vec3 topLeft = { pos.x - 2.0f, pos.y + 10.0f, pos.z };
	glm::vec3 bottomLeft = { pos.x - 2.0f, pos.y + 6.0f, pos.z };
	glm::vec3 topRight = { pos.x + 2.0f, pos.y + 10.0f, pos.z };
	glm::vec3 bottomRight = { pos.x + 2.0f, pos.y + 6.0f, pos.z };

	CoinPositions = {
		topLeft,
		bottomLeft,
		topRight,
		bottomRight };

	CoinIndices = {
		glm::vec3(2, 0, 1),
		glm::vec3(1, 2, 3)
	};
	Coin::bindToModel();
}


Coin::~Coin()
{
}

void Coin::bindToModel() {
	RawModel temp = Loader::loadToVAO(CoinPositions, CoinIndices);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();
}

bool Coin::isCoinTouched(Coin c, glm::vec3 point) {
	float hitBoxPadding = 2.0f;
	//between its x values
	if (((point.x > c.CoinPositions[0].x - hitBoxPadding) && (point.x < c.CoinPositions[2].x + hitBoxPadding))){
		//between its y values
		if (((point.y < c.CoinPositions[0].y + hitBoxPadding) && (point.y > c.CoinPositions[1].y - hitBoxPadding))){
			//is close to Z
			if ((point.z < c.CoinPositions[0].z + hitBoxPadding) && (point.z > c.CoinPositions[0].z - hitBoxPadding)){
				return true;
			}
		}
	}
	return false;
}