#include "Vehicle.h"
#include "Loader.h"

void Vehicle::build() {
	positions = {
		vec3(0.5f, 0.0f, 0.0f), //Front	Left 0
		vec3(-0.5f, 0.0f, 0.0f), //Front	Right 1
		vec3(0.5f, 0.4f, 0.1f), //Hood	Left 2
		vec3(-0.5f, 0.4f, 0.1f), //Hood	Right 3
		vec3(0.5f, 0.45f, 0.5f), //WS	Left 4
		vec3(-0.5f, 0.45f, 0.5f), //WS	Right 5
		vec3(0.5f, 0.7f, 0.6f), //Roof	Left 6
		vec3(-0.5f, 0.7f, 0.6f), //Roof	Right 7
		vec3(0.5f, 0.7f, 1.4f), //Roof	Back Left 8
		vec3(-0.5f, 0.7f, 1.4f), //Roof	Back Right 9
		vec3(0.5f, 0.0f, 1.5f), //Back	Left 10
		vec3(-0.5f, 0.0f, 1.5f) //Back	Right 11
		//vec3(0.0f, 0.5f, 0.8f), //Top	2
		//vec3(0.4f, 0.0f, 0.8f), //Left	3
		//vec3(-0.4f, 0.0f, 0.8f) //Right	4
	};

	indices = {
		vec3(0, 1, 10),
		vec3(1, 10, 11),
		vec3(0, 1, 2),
		vec3(1, 2, 3),
		vec3(2, 3, 4),
		vec3(3, 4, 5),
		vec3(4, 5, 6),
		vec3(5, 6, 7),
		vec3(6, 7, 8),
		vec3(7, 8, 9),
		vec3(8, 9, 10),
		vec3(9, 10, 11),
		vec3(0,2,4),
		vec3(0,4,10),
		vec3(4,10,8),
		vec3(4,8,6),
		vec3(1, 3, 5),
		vec3(1, 5, 11),
		vec3(5, 11, 9),
		vec3(5, 9, 7)
	};

	normals = {
		glm::vec3(0.785367, 0.487923, -0.380959),
		glm::vec3(1, 0, 0),
		glm::vec3(0.707107, 0.701646, -0.0877058),
		glm::vec3(0.707107, -0.171499, 0.685994),
		glm::vec3(0, 0.371391, -0.928477),
		glm::vec3(0, -0.992278, 0.124035),
		glm::vec3(-0.707107, 0.707107, 0),
		glm::vec3(-0.707107, -0.262613, 0.656532),
		glm::vec3(-0.894427, 0.0632456, 0.442719),
		glm::vec3(-0.894427, -0.447214, 0),
		glm::vec3(0, -0.755454, -0.655202),
		glm::vec3(0, -0.755454, -0.655202)
	};

	colours = {
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f)
	};
}

void Vehicle::bindToModel() {
	RawModel temp = Loader::loadToVAO(positions, normals, indices, colours);
	this->vaoID = temp.getVAOID();
	this->vertexCount = temp.getVertexCount();

	//Loader::generateNormals(positions, indices);


}

mat4 Vehicle::getModelMatrix() {
	return modelMatrix;
}

void Vehicle::tick() {
	modelMatrix = translate(modelMatrix, vec3(0.0f, 0.0f, -0.001f));

	vec3 position(modelMatrix[3]);

	if (horizontal)
	{
		for (unsigned i = 0; i < streets->size(); i++){
			Street test = streets->at(i);
			if (current_street != &streets->at(i) && last_street != &streets->at(i) && test.topLeft.z < position.z && test.bottomRight.z > position.z)
			{
				float width = test.bottomRight.z - test.topLeft.z;

				if (abs(width) > 20)
				{
					continue;
				}

				modelMatrix = translate(mat4(), vec3(position.x, 0.5f, test.topLeft.z + width / 2));
				modelMatrix = glm::rotate(modelMatrix, -3.14f / 2.0f, vec3(0.0f, 0.1f, 0.0f));
				modelMatrix = scale(modelMatrix, vec3(100, 100, 100));
				horizontal = false;

				last_street = current_street;
				current_street = &streets->at(i);
				break;
			}
		}
	}
	else
	{
		for (unsigned i = 0; i < streets->size(); i++){
			Street test = streets->at(i);

			if (current_street != &streets->at(i) && last_street != &streets->at(i) && test.topLeft.x < position.x && test.bottomRight.x > position.x)
			{
				float width = test.bottomRight.x - test.topLeft.x;

				if (abs(width) > 20)
				{
					continue;
				}

				modelMatrix = translate(mat4(), vec3(test.bottomRight.x - width / 2, 0.5f, position.z));
				modelMatrix = glm::rotate(modelMatrix, -3.14f, vec3(0.0f, 0.1f, 0.0f));
				modelMatrix = scale(modelMatrix, vec3(100, 100, 100));
				horizontal = true;

				last_street = current_street;
				current_street = &streets->at(i);
				break;
			}
		}
	}
}

void Vehicle::assignStreet(vector<Street>* streets) {
	this->streets = streets;

	int randomIndex = rand() % streets->size();

	current_street = &(streets->at(randomIndex));

	Street street(*current_street);

	float width1 = abs(street.bottomRight.x - street.topLeft.x);
	float width2 = abs(street.topLeft.z - street.bottomRight.z);
	float width;

	if (width1 < width2)
	{
		horizontal = true;
		width = width1;
	}
	else
	{
		horizontal = false;
		width = width2;
	}

	if (horizontal)
	{
		modelMatrix = translate(mat4(), vec3(street.bottomRight.x - width / 2.0f, 0.5f, street.bottomRight.z));
		modelMatrix = glm::rotate(modelMatrix, -3.14f, vec3(0.0f, 0.1f, 0.0f));
	}
	else
	{
		modelMatrix = translate(mat4(), vec3(street.bottomRight.x, 0.5f, street.bottomRight.z + width / 2.0f));
		modelMatrix = glm::rotate(modelMatrix, -3.14f / 2.0f, vec3(0.0f, 0.1f, 0.0f));
	}
	

	modelMatrix = scale(modelMatrix, vec3(100, 100, 100));

	
}