#include "glew.h"		// include GL Extension Wrangler
#include "glfw3.h"  // include GLFW helper library
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "gtc/constants.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream> 
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <gtx/rotate_vector.hpp>

// Custom Defined headers
#include "../VS2013/RawModel.h"
#include "../VS2013/Loader.h"
#include "../VS2013/Building.h"
#include "../VS2013/World.h"
#include "../VS2013/Street.h"
#include "../VS2013/Vehicle.h"
#include "../VS2013/Coin.h"

using namespace std;
using namespace glm;

#define M_PI        3.14159265358979323846264338327950288f   /* pi */
#define DEG_TO_RAD	M_PI/180.0f

GLFWwindow* window = 0x00;

GLuint shader_program = 0;

GLuint view_matrix_id = 0;
GLuint model_matrix_id = 0;
GLuint proj_matrix_id = 0;


///Transformations
glm::mat4 proj_matrix;
glm::mat4 view_matrix;
glm::mat4 model_matrix;

// Given a 3D environment
GLfloat point_size = 3.0f;

//Window resize
GLuint WIDTH = 800;
GLuint HEIGHT = 800;

vector<RawModel*> models;

void window_resize_callback(GLFWwindow* window, int width, int height){
	WIDTH = width;
	HEIGHT = height;
	//reset view
	glViewport(0, 0, WIDTH, HEIGHT);
}

glm::vec3 cameraPosition = glm::vec3(0, 100, -10);
glm::vec3 direction, Vright, up;
float horizontalAngle = 0.0f;
float verticleAngle = 0.0f;
float initialFoV = 45.0f;
float speed = 100.0f;
int mouseSpeed = 1.0f;
double xpos = 0, ypos = 0;
double currentTime = 0, lastTime = 0;
float deltaTime = 0.0f;
bool pauseCam = false;
vector<Vehicle> vehicles;

// Movement variables
bool leftKey = false, rightKey = false, upKey = false, downKey = false, noclip = false;
void key_callback(GLFWwindow *_window, int key, int scancode, int action, int mods){
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_LEFT:
	case GLFW_KEY_A:
		if (!pauseCam){
			if (action == GLFW_PRESS){
				leftKey = true;
			}
		}
		if (action == GLFW_RELEASE){
			leftKey = false;
		}
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_D:
		if (!pauseCam){
			if (action == GLFW_PRESS){
				rightKey = true;
			}
		}
		if (action == GLFW_RELEASE){
			rightKey = false;
		}
		break;
	case GLFW_KEY_UP:
	case GLFW_KEY_W:
		if (!pauseCam){
			if (action == GLFW_PRESS){
				upKey = true;
			}
		}
		if (action == GLFW_RELEASE){
			upKey = false;
		}
		break;
	case GLFW_KEY_DOWN:
	case GLFW_KEY_S:
		if (!pauseCam){
			if (action == GLFW_PRESS){
				downKey = true;
			}
		}
		if (action == GLFW_RELEASE){
			downKey = false;
		}
		break;
	case GLFW_KEY_N:
		if (action == GLFW_PRESS){
			noclip = !noclip;
		}
	case GLFW_KEY_T:
		if (action == GLFW_PRESS){
			pauseCam = false;
			leftKey = false;
			rightKey = false;
			upKey = false;
			downKey = false;
		}
	default: break;
	}

}

bool initialize() {
	/// Initialize GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	/// Create a window of size 800x800
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "SOUVLAKI CITY", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}
	//register callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_resize_callback);

	glfwMakeContextCurrent(window);

	/// Initialize GLEW extension handler
	glewExperimental = GL_TRUE;	///Needed to get the latest version of OpenGL
	glewInit();

	/// Get the current OpenGL version
	const GLubyte* renderer = glGetString(GL_RENDERER); /// Get renderer string
	const GLubyte* version = glGetString(GL_VERSION); /// Version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/// Enable the depth test i.e. draw a pixel if it's closer to the viewer
	glEnable(GL_DEPTH_TEST); /// Enable depth-testing
	glDepthFunc(GL_LESS);	/// The type of testing i.e. a smaller value as "closer"

	//Seed random number generation
	srand(static_cast <unsigned> (time(0)));

	return true;
}

GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path)	{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	//The three variables below hold the id of each of the variables in the shader
	//If you read the vertex shader file you'll see that the same variable names are used.
	view_matrix_id = glGetUniformLocation(ProgramID, "view_matrix");
	model_matrix_id = glGetUniformLocation(ProgramID, "model_matrix");
	proj_matrix_id = glGetUniformLocation(ProgramID, "proj_matrix");

	return ProgramID;
}

void render(RawModel* model){
	glBindVertexArray(model->getVAOID());
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

template <typename T>
void remove(std::vector<T>& vec, size_t pos)
{
	std::vector<T>::iterator it = vec.begin();
	std::advance(it, pos);
	vec.erase(it);
}

int main() {
	initialize();

	

	int nbCoins = 0;
	int nbCollectedCoins = 0;

	///Load the shaders
	shader_program = loadShaders("../Source/COMP371_hw1.vs", "../Source/COMP371_hw1.fss");

	//Set the camera
	view_matrix = glm::translate(view_matrix, glm::vec3(0, 0, -10)); //Camera's cameraPosition
	proj_matrix = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f); //Camera's "lense"

	//create RawModel based on vertex and index data
	glm::vec3 farLeftMain = { -500.0f, 0.0f, 500.0f };
	glm::vec3 bottomRightMain = { 500.0f, 0.0f, -500.0f };
	float xOffset = (bottomRightMain.x - farLeftMain.x)/100; // 1000 lanes exist with this width
	float zOffset = -(bottomRightMain.z - farLeftMain.z) /100; // 1000 lanes exist with this width

	//creating object containers
	vector<Street> streetList;

	vector<float> streetXList;
	vector<float> streetZList;
	vector<Coin> coinList;

	// BUILDING OBJECTS!
	World world = World(farLeftMain, bottomRightMain);
	Street street = Street({ -500.0f, 1.0f, 500.0f }, { -490.0f, 1.0f, -500.0f });

	//Pushing x axis streets
	for (float i = farLeftMain.x; i < bottomRightMain.x; i += xOffset * 10){
		Street* s = new Street({ i, 1.0f, farLeftMain.z }, { i + xOffset, 1.0f, bottomRightMain.z });
		streetList.push_back(*s);
		models.push_back(s);
		streetXList.push_back(i + xOffset);
		Coin c = Coin(vec3{ i + (xOffset / 2.0f), 0.0f, farLeftMain.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (bottomRightMain.z - farLeftMain.z))) });
		coinList.push_back(c);
		nbCoins++;
	}
	//Pushing z axis streets
	for (float j = bottomRightMain.z; j < farLeftMain.z; j += zOffset * 10){
		Street* s = new Street({bottomRightMain.x, 1.0f, j}, {farLeftMain.x, 1.0f, j + zOffset});
		streetList.push_back(*s);
		models.push_back(s);
		streetZList.push_back(j + zOffset);
		Coin c = Coin(vec3{ farLeftMain.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (bottomRightMain.x - farLeftMain.x))), 0.0f, j + (zOffset / 2.0f) });
		coinList.push_back(c);
		nbCoins++;
	}
	for (int x = 0; x < streetXList.size(); x++){
		for (int z = 0; z < streetZList.size(); z++){
			vector <Building> thisBlockBuildings;
			for (int nb = 0; nb < 50; nb++){
				//generate 20 buildings per block
				float lowX = streetXList[x];
				float highX = streetXList[x] + xOffset * 10.0f;
				float lowZ = streetZList[z];
				float highZ = streetZList[z] + zOffset * 10.0f;
				float bX = lowX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highX - lowX)));
				float bZ = lowZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (highZ - lowZ)));
				glm::vec3 blockPlacement = { bX, 0.0f, bZ };
				Building* b = Building::generateRandomBuilding(blockPlacement, xOffset * 10, glm::vec2{ (streetXList[x] / (xOffset * 10.0f)), (streetZList[z] / (zOffset * 10.0f)) });
				int nbFailures = 0;
				while (nbFailures < 10){
					if (Building::checkIfConflict(*b, thisBlockBuildings, streetXList[x], streetZList[z], xOffset, zOffset)){
						thisBlockBuildings.push_back(*b);
						//buildingList.push_back(b);
						models.push_back(b);
						break;
					}
					else{
						nbFailures++;
					}
				}

			}
		}
	}

	for (int i = 0; i < 100; i++)
	{
		Vehicle vehicle;
		vehicle.assignStreet(&streetList);
		vehicle.build();
		vehicle.bindToModel();
		
		vehicles.push_back(vehicle);
	}
	

	glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
	noclip = false;
	float tempAngle = 0.0f;
	while (!glfwWindowShouldClose(window)) {

		//Getting Time data
		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

		//Determine cursor cameraPosition and angle
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
		if (!pauseCam){
			horizontalAngle += mouseSpeed * deltaTime * (float((WIDTH / 2.0f) - xpos));
			tempAngle += mouseSpeed * deltaTime * (float((HEIGHT / 2.0f) - ypos));
			if (tempAngle < (3.14f / 2.0f) && tempAngle >(-3.14f / 2.0f)){
				verticleAngle = tempAngle;
			}
			tempAngle = verticleAngle;
		}
		tempAngle = verticleAngle;

		vec3 oldCameraPos(cameraPosition);

		//Incrementing cameraPosition
		if (upKey){
			if (noclip){
				cameraPosition += direction * deltaTime * speed;
			}
			else if (!noclip){
				cameraPosition += vec3(direction.x, 0, direction.z) * deltaTime * speed;
			}
		}
		else if (downKey){
			if (noclip){
				cameraPosition -= direction * deltaTime * speed;
			}
			else if (!noclip){
				cameraPosition -= vec3(direction.x, 0, direction.z) * deltaTime * speed;
			}
		}
		if (leftKey){
			cameraPosition -= Vright * deltaTime * speed;
		}
		else if (rightKey){
			cameraPosition += Vright * deltaTime * speed;
		}

		bool buildingHit = false;
		for (int j = 0; j < models.size(); j++)
		{
			if (!models[j]->isPointLegal(cameraPosition))
			{
				buildingHit = true;
				break;
			}
		}

		if (buildingHit || !world.isPointLegal(cameraPosition))
		{
			cameraPosition = oldCameraPos;
		}

		for (int j = 0; j < coinList.size(); j++)
		{
			if (Coin::isCoinTouched(coinList[j], cameraPosition))
			{
				//remove coin
				remove(coinList, j);
				//increase coin count
				nbCollectedCoins++;
				break;
			}
		}

		//WIN GAME!
		if (nbCollectedCoins == nbCoins){
			//DISPLAY YOU WIN!
			pauseCam = true;

			nbCollectedCoins = 0;
		}


		direction = vec3(cos(verticleAngle) * sin(horizontalAngle), sin(verticleAngle), cos(verticleAngle) * cos(horizontalAngle));
		Vright = vec3(sin(horizontalAngle - (3.14f / 2.0f)), 0, cos(horizontalAngle - (3.14f / 2.0f)));
		up = cross(Vright, direction);

		view_matrix = lookAt(cameraPosition, cameraPosition + direction, up);


		// Clear Screen with color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
		glPointSize(point_size);

		glUseProgram(shader_program);

		//Pass the values of the three matrices to the shaders
		glUniformMatrix4fv(proj_matrix_id, 1, GL_FALSE, value_ptr(proj_matrix));
		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, value_ptr(view_matrix));
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, value_ptr(model_matrix));

		// Render all models
		for (int k = 0; k < models.size(); k++){
			render(models[k]);
		}

		render(&world);

		for (int j = 0; j < coinList.size(); j++){
			glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, value_ptr(*coinList[j].coinModel));
			Coin::rotateToFace(coinList[j], cameraPosition, Vright);
			render(&coinList[j]);
			

		}
	
		for (unsigned j = 0; j < vehicles.size(); j++)
		{
			vehicles[j].tick();
			mat4 vecModel(vehicles[j].getModelMatrix());
			glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, value_ptr(vecModel));
			render(&vehicles[j]);
		}

		// Update other events like input handling
		glfwPollEvents();
		// Put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		//Setting lastTime
		lastTime = currentTime;
	}

	Loader::cleanUp();
	return 0;
}