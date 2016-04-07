#include "Coin.h"
#include "Loader.h"

//remove as not needed, just copied everything used in main
using namespace std;
using namespace glm;

vector<vec3> CoinPositions, CoinIndices, Coincolors, Coinnormals;
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

	Coincolors = {
		vec3(1,0,0),
		vec3(1, 0, 0),
		vec3(1, 0, 0),
		vec3(1, 0, 0)
	};

	Coinnormals = {
		vec3(0,0,1),
		vec3(0, 0, 1),
		vec3(0, 0, 1),
		vec3(0, 0, 1)
	};




	Coin::bindToModel();
	
	mycoinPosition = pos;

	coinModel = new mat4();
}


Coin::~Coin()
{
}

void Coin::bindToModel() {
	RawModel temp = Loader::loadToVAO(CoinPositions, Coinnormals , CoinIndices, Coincolors);
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

void Coin::rotateToFace(Coin c, glm::vec3 point){
	
	*(c.coinModel) = translate(*(c.coinModel), c.mycoinPosition);
	*(c.coinModel) = glm::rotate(*(c.coinModel), 2.14f, glm::vec3(0.0f, 1.0f, 0.0f));
	*(c.coinModel) = translate(*(c.coinModel), -c.mycoinPosition);

}