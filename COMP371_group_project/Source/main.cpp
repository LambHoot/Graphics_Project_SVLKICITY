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

using namespace std;
using namespace glm;

#define M_PI        3.14159265358979323846264338327950288f   /* pi */
#define DEG_TO_RAD	M_PI/180.0f

GLFWwindow* window = 0x00;

GLuint shader_program = 0;

GLuint view_matrix_id = 0;
GLuint model_matrix_id = 0;
GLuint proj_matrix_id = 0;

GLuint drawType_id = 0;
GLuint camPos_id = 0;


///Transformations
mat4 proj_matrix;
mat4 view_matrix;
mat4 model_matrix;

// Given a 3D environment
GLfloat point_size = 3.0f;

//Window resize
GLuint WIDTH = 800;
GLuint HEIGHT = 800;
void window_resize_callback(GLFWwindow* window, int width, int height){
	WIDTH = width;
	HEIGHT = height;
	//reset view
	glViewport(0, 0, WIDTH, HEIGHT);
}

vec3 cameraPosition = vec3(0, 1, -10);
vec3 direction, Vright, up;
float horizontalAngle = 0.0f;
float verticleAngle = 0.0f;
float initialFoV = 45.0f;
float speed = 10.0f;
int mouseSpeed = 1.0f;
double xpos = 0, ypos = 0;
double currentTime = 0, lastTime = 0;
float deltaTime = 0.0f;

void loadTexture(){



}

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
		if (action == GLFW_PRESS){
			leftKey = true;
		}
		else if (action == GLFW_RELEASE){
			leftKey = false;
		}
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS){
			rightKey = true;
		}
		else if (action == GLFW_RELEASE){
			rightKey = false;
		}
		break;
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS){
			upKey = true;
		}
		else if (action == GLFW_RELEASE){
			upKey = false;
		}
		break;
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS){
			downKey = true;
		}
		else if (action == GLFW_RELEASE){
			downKey = false;
		}
		break;
	case GLFW_KEY_N:
		if (action == GLFW_PRESS){
			noclip = !noclip;
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

	glBindAttribLocation(ProgramID, 0, "in_cameraPosition");

	//appearing in the vertex shader.
	glBindAttribLocation(ProgramID, 1, "in_Color");

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
	drawType_id = glGetUniformLocation(ProgramID, "drawType");
	camPos_id = glGetUniformLocation(ProgramID, "camPos");

	return ProgramID;
}

void render(RawModel model){
	glBindVertexArray(model.getVAOID());
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, (void*)0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

int main() {
	initialize();

	bool swap = true;

	///Load the shaders
	shader_program = loadShaders("../Source/COMP371_hw1.vs", "../Source/COMP371_hw1.fss");

	//Set the camera
	view_matrix = translate(view_matrix, vec3(0, 0, -10)); //Camera's cameraPosition
	proj_matrix = perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f); //Camera's "lense"

	//create RawModel based on vertex and index data
	glm::vec3 farLeftMain = { -500.0f, 0.0f, 500.0f };
	glm::vec3 bottomRightMain = { 500.0f, 0.0f, -500.0f };
	float xOffset = (bottomRightMain.x - farLeftMain.x)/1000; // 1000 lanes exist with this width
	float zOffset = -(bottomRightMain.z - farLeftMain.z) / 1000; // 1000 lanes exist with this width
	// 10 streets will exist in each direction
	Building building = Building(5.0f, 1.0f);
	World world = World(farLeftMain, bottomRightMain);
	Street street = Street({ -500.0f, 1.0f, 500.0f }, { -490.0f, 1.0f, -500.0f });
	vector<Street> streetList;

	//Pushing x axis streets
	for (float i = farLeftMain.x; i < bottomRightMain.x; i += xOffset * 10){
		Street s = Street({ i, 1.0f, farLeftMain.z }, { i + xOffset, 1.0f, bottomRightMain.z });
		streetList.push_back(s);
	}
	//Pushing z axis streets
	for (float j = bottomRightMain.z; j < farLeftMain.z; j += zOffset * 10){
		Street s = Street({bottomRightMain.x, 1.0f, j}, {farLeftMain.x, 1.0f, j + zOffset});
		streetList.push_back(s);
	}

	

	glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
	noclip = false;
	while (!glfwWindowShouldClose(window)) {
		
		glUniform1i(drawType_id, 0);
		glUniform3f(camPos_id, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		//Getting Time data
		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);

		//Determine cursor cameraPosition and angle
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, (WIDTH / 2), (HEIGHT / 2));
		horizontalAngle += mouseSpeed * deltaTime * (float((WIDTH / 2.0f) - xpos));
		verticleAngle += mouseSpeed * deltaTime * (float((HEIGHT / 2.0f) - ypos));

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

		direction = vec3(cos(verticleAngle) * sin(horizontalAngle), sin(verticleAngle), cos(verticleAngle) * cos(horizontalAngle));
		Vright = vec3(sin(horizontalAngle - (3.14f / 2.0f)), 0, cos(horizontalAngle - (3.14f / 2.0f)));
		up = cross(Vright, direction);

		view_matrix = lookAt(cameraPosition, cameraPosition + direction, up);


		// Clear Screen with color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glPointSize(point_size);

		glUseProgram(shader_program);

		//Pass the values of the three matrices to the shaders
		glUniformMatrix4fv(proj_matrix_id, 1, GL_FALSE, value_ptr(proj_matrix));
		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, value_ptr(view_matrix));
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, value_ptr(model_matrix));

		// Rendering. TODO: foreach loop of RawModels in scene
		render(building);
		render(world);
		glUniform1i(drawType_id, 1);
		//render(street);

		for (int j = 0; j < streetList.size(); j++){
			render(streetList[j]);
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