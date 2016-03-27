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

#include "../VS2013/RawModel.h"

using namespace std;

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

float translateSensitivityX = 0.005f;
float translateSensitivityY = 0.005f;
float isPressedy = 0.0f;
float isPressedx = 0.0f;
void key_callback(GLFWwindow *_window, int key, int scancode, int action, int mods){
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS){
			isPressedx = translateSensitivityX;
		}
		else if (action == GLFW_RELEASE) {
			isPressedx = 0.0f;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS){
			isPressedx = -translateSensitivityX;
		}
		else if (action == GLFW_RELEASE) {
			isPressedx = 0.0f;
		}
		break;
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS){
			isPressedy = translateSensitivityY;
		}
		else if (action == GLFW_RELEASE) {
			isPressedy = 0.0f;
		}
		break;
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS){
			isPressedy = -translateSensitivityY;
		}
		else if (action == GLFW_RELEASE) {
			isPressedy = 0.0f;
		}
		break;
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

//to be removed and replaced with arrays;
vector<GLuint> VAO, VBO;
GLuint EBO; //still unused

bool cleanUp() {
	glDisableVertexAttribArray(0);
	//Properly de-allocate all resources once they've outlived their purpose

	GLuint *vao, *vbo;
	for (int i = 0; i < VAO.size(); i++){
		vao = &VAO[i];
		cout << "Deleting VAO id = " << vao << endl;
		glDeleteVertexArrays(1,vao);
	}
	for (int i = 0; i < VBO.size(); i++){
		vbo = &VBO[i];
		cout << "Deleting VBO id = " << vbo << endl;
		glDeleteBuffers(1, vbo);
	}
	
	glDeleteBuffers(1, &EBO);

	// Close GL context and any other GLFW resources
	glfwTerminate();

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

	glBindAttribLocation(ProgramID, 0, "in_Position");

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

	return ProgramID;
}

void storeDataInAttribList(int attNumber, GLfloat list[], int data_size){
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data_size, list, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(attNumber);
	glVertexAttribPointer(
		attNumber,
		3,                  // 3 for 3D
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // Distance between vertices in array
		(void*)0            // array buffer offset
		);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	VBO.push_back(vbo);

	//TODO: NOTE: vbo memory tracking not currently implemented. Once many are made, they must be deleted when done.
}

void bindIndicesBuffer(GLuint indices[], int data_size){
	GLuint vbo;
	glGenBuffers(0, &vbo);
	VBO.push_back(vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); ??
}


RawModel loadToVAO(GLfloat positions[], int positions_length, GLuint indices[], int indices_length){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	bindIndicesBuffer(indices, sizeof(indices)*indices_length);
	storeDataInAttribList(0, positions, sizeof(positions)*positions_length);
	glBindVertexArray(0);

	VAO.push_back(vao);

	//TODO: NOTE: vao memory tracking not currently implemented. Once many are made, they must be deleted when done.
	return RawModel(vao, indices, indices_length); // An Okay Constructor?
}

void render(RawModel model){
	glBindVertexArray(model.getVAOID());
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, model.getVertexCount(), GL_UNSIGNED_INT, model.getIndices());
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

//for debug -- raw data
// An array of 4 vectors which represents 4 vertices to make a box
GLfloat triangle[] = {
	-0.5f, 0.5f, 0,
	-0.5f, -0.5f, 0,
	0.5f, -0.5f, 0,
	0.5f, 0.5f, 0.0f
};

GLuint indices[] = {
	0, 1, 3,
	3, 1, 2
};

int main() {

	initialize();

	///Load the shaders
	shader_program = loadShaders("../Source/COMP371_hw1.vs", "../Source/COMP371_hw1.fss");

	//Set the camera
	view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -10.0f)); //Camera's position
	proj_matrix = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f); //Camera's "lense"

	//create data
	RawModel triModel = loadToVAO(triangle, sizeof(triangle) / sizeof(*triangle), indices, sizeof(indices)/sizeof(*indices));

	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
		glPointSize(point_size);

		//translating camera on the x and y axis
		/*if ((isPressedy != 0.0f) || (isPressedx != 0.0f)){
			view_matrix = glm::translate(view_matrix, glm::vec3(isPressedx, -isPressedy, 0.0f));
		}*/

		if ((isPressedy != 0.0f) || (isPressedx != 0.0f)){
			model_matrix = glm::rotate(model_matrix, 0.01f, glm::vec3(isPressedx, isPressedy, 0.0f));
		}

		glUseProgram(shader_program);

		//Pass the values of the three matrices to the shaders
		glUniformMatrix4fv(proj_matrix_id, 1, GL_FALSE, glm::value_ptr(proj_matrix));
		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, glm::value_ptr(model_matrix));

		//DRAW HERE
		render(triModel);

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	cleanUp();
	return 0;
}