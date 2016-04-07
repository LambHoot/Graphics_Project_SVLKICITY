#include "Vehicle.h"
#include "Loader.h"

void Vehicle::build() {

	float myrand = rand() % 10;
	myspeed = (myrand - 5.0f) / 100;


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
		glm::vec3(0.535666, 0.665584, -0.519672),
		glm::vec3(1, 0, 0),
		glm::vec3(0.707107, 0.701646, -0.0877058),
		glm::vec3(0.707107, -0.171499, 0.685994),
		glm::vec3(0, 0.371391, -0.928477),
		glm::vec3(0, -0.992278, 0.124035),
		glm::vec3(-0.707107, 0.707107, 0),
		glm::vec3(-0.707107, -0.262613, 0.656532),
		glm::vec3(-0.707107, 0.1, 0.7),
		glm::vec3(-0.707107, -0.707107, 0),
		glm::vec3(0, -0.755454, -0.655202),
		glm::vec3(0, -0.755454, -0.655202)
	};

	mycolour = vec3(
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
		);

	mycolour = vec3(1.0f,0.0f,0.0f);

	colours = {
		mycolour,
		mycolour,
		mycolour,
		mycolour,
		mycolour, 
		mycolour,
		mycolour,
		mycolour,
		mycolour,
		mycolour,
		mycolour,
		mycolour
	};

	//normals = Loader::generateNormals(positions, indices);
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

	

	vec3 position(modelMatrix[3]);

	if (position.x < -500.0f || position.x > 500.0f || position.z < -500.0f || position.z > 500.0f)
	{
		assignStreet(streets);
	}

	float cos = modelMatrix[2][2];
	float sin = modelMatrix[1][2];

	if (!(targetRotation + 0.01 > currentRotation && targetRotation - 0.01 < currentRotation))
	{
		if (targetRotation < currentRotation)
		{
			currentRotation -= 0.01;
		}
		else
		{
			currentRotation += 0.01;
		}
		
		modelMatrix = rotate(oldModelMatrix, currentRotation, vec3(0.0f, 1.0f, 0.0f));
		rotating = true;
		return;
	}
	else if (rotating)
	{
		rotating = false;
		modelMatrix = rotate(oldModelMatrix, targetRotation, vec3(0.0f, 1.0f, 0.0f));
	}

	modelMatrix = translate(modelMatrix, vec3(0.0f, 0.0f, -0.1f + myspeed));

	if (mytick < 10)
	{
		mytick++;
	}

	mytick = 0;

	int turnDescision = rand() % 3;

	if (horizontal)
	{
		for (unsigned i = 0; i < streets->size(); i++){
			Street test = streets->at(i);
			if (current_street != &streets->at(i) && last_street != &streets->at(i) && test.topLeft.z + 4.9f < position.z && test.bottomRight.z - 4.9f > position.z)
			{
				if (last_street == nullptr || turnDescision == 0)
				{
					last_street = &streets->at(i);
					return;
				}

				float width = test.bottomRight.z - test.topLeft.z;

				if (abs(width) > 20)
				{
					continue;
				}

				modelMatrix = translate(mat4(), vec3(position.x, HOVER, test.topLeft.z + width / 2));
				modelMatrix = glm::rotate(modelMatrix, endRotation, vec3(0.0f, 0.1f, 0.0f));
				//modelMatrix = scale(modelMatrix, vec3(100, 100, 100));
				horizontal = false;

				if (turnDescision == 1)
				{
					targetRotation = 3.14f / 2.0f;
					currentRotation = 0;
					endRotation = -3.14f / 2.0f;
				}
				else
				{
					targetRotation = -3.14f / 2.0f;
					currentRotation = 0;
					endRotation = 3.14f / 2.0f;
				}

				oldModelMatrix = modelMatrix;
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

			if (current_street != &streets->at(i) && last_street != &streets->at(i) && test.topLeft.x + 4.9f < position.x && test.bottomRight.x - 4.9f > position.x)
			{
				
				if (last_street == nullptr || turnDescision == 0)
				{
					last_street = &streets->at(i);
					return;
				}

				float width = test.bottomRight.x - test.topLeft.x;

				if (abs(width) > 20)
				{
					continue;
				}

				modelMatrix = translate(mat4(), vec3(test.bottomRight.x - width / 2, HOVER, position.z));
				modelMatrix = glm::rotate(modelMatrix, endRotation, vec3(0.0f, 0.1f, 0.0f));
				
				rotating = true;

				//modelMatrix = scale(modelMatrix, vec3(100, 100, 100));
				oldModelMatrix = modelMatrix;
				
				if (turnDescision == 1)
				{
					currentRotation = 0;
					targetRotation = -3.14f / 2.0f;
					endRotation = 3.14f;
				}
				else
				{
					currentRotation = 0;
					targetRotation = 3.14f / 2.0f;
					endRotation = -3.14f;
				}


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
		modelMatrix = translate(mat4(), vec3(street.bottomRight.x - width / 2.0f, HOVER, street.bottomRight.z));
		modelMatrix = glm::rotate(modelMatrix, 3.14f, vec3(0.0f, 0.1f, 0.0f));
		endRotation = 3.14f;
		//mycolour = vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		modelMatrix = translate(mat4(), vec3(street.bottomRight.x, HOVER, street.bottomRight.z - width / 2.0f));
		modelMatrix = glm::rotate(modelMatrix, -3.14f / 2.0f, vec3(0.0f, 0.1f, 0.0f));
		endRotation = -3.14f / 2.0f;
		//mycolour = vec3(1.0f, 1.0f, 0.0f);
	}
	

	//modelMatrix = scale(modelMatrix, vec3(100, 100, 100));

	
}