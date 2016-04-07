#define _CRT_SECURE_NO_DEPRECATE

#include "Loader.h"
vector<GLuint> Loader::VAO;
vector<GLuint> Loader::VBO;


Loader::Loader()
{
}


Loader::~Loader()
{
}

void Loader::bindIndicesBuffer(GLuint indices[], int data_size){
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, indices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);// added by phil -- to be seen if any trouble caused

	VBO.push_back(vbo);
}

void Loader::storeDataInAttribList(int attNumber, GLfloat list[], int data_size){
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
}

RawModel Loader::loadToVAO(vector <glm::vec3> positions, vector <glm::vec3> normals, vector<glm::vec3> indices, vector<glm::vec3> colors) {
	GLfloat *pos = new GLfloat[positions.size() * 3];
	GLuint *ind = new GLuint[indices.size() * 3];
	GLfloat *col = new GLfloat[colors.size() * 3];
	GLfloat *nor = new GLfloat[normals.size() * 3];

	for (unsigned int i = 0; i < positions.size(); i++){
		pos[3 * i + 0] = positions[i].x;
		pos[3 * i + 1] = positions[i].y;
		pos[3 * i + 2] = positions[i].z;
	}

	for (unsigned int j = 0; j < indices.size(); j++){
		ind[3 * j + 0] = indices[j].x;
		ind[3 * j + 1] = indices[j].y;
		ind[3 * j + 2] = indices[j].z;
	}

	for (unsigned int j = 0; j < colors.size(); j++){
		col[3 * j + 0] = colors[j].x;
		col[3 * j + 1] = colors[j].y;
		col[3 * j + 2] = colors[j].z;
	}

	for (unsigned int j = 0; j < colors.size(); j++){
		col[3 * j + 0] = colors[j].x;
		col[3 * j + 1] = colors[j].y;
		col[3 * j + 2] = colors[j].z;
	}

	return loadToVAO(pos, positions.size() * point_size, nor, normals.size() * point_size, ind, indices.size() * point_size, col, colors.size()*point_size);
}

RawModel Loader::loadToVAO(GLfloat positions[], int positions_length, GLfloat normals[], int normals_length, GLuint indices[], int indices_length, GLfloat colors[], int colors_length){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	bindIndicesBuffer(indices, sizeof(indices)*indices_length);
	storeDataInAttribList(0, positions, sizeof(positions)*positions_length);
	storeDataInAttribList(1, normals, sizeof(positions)*positions_length);
	storeDataInAttribList(2, colors, sizeof(colors)*colors_length);


	//storeDataInAttribList(3, colors, sizeof(colors)*colors_length);
	glBindVertexArray(0);

	VAO.push_back(vao);

	return RawModel(vao, indices_length);
}

bool Loader::cleanUp() {
	glDisableVertexAttribArray(0);
	//Properly de-allocate all resources once they've outlived their purpose

	GLuint *vao, *vbo;

	cout << "Deleting VAOs..." << endl;
	for (unsigned int i = 0; i < VAO.size(); i++){
		vao = &VAO[i];
		//cout << "Deleting VAO id = " << vao << endl;
		glDeleteVertexArrays(1, vao);
	}

	cout << "Deleting VBOs..." << endl;
	for (unsigned int i = 0; i < VBO.size(); i++){
		vbo = &VBO[i];
		//cout << "Deleting VBO id = " << vbo << endl;
		glDeleteBuffers(1, vbo);
	}

	// Close GL context and any other GLFW resources
	glfwTerminate();
	return true;
}

vector < glm::vec3> Loader::generateNormals(vector<glm::vec3> vertices, vector<glm::vec3> indices){
	vector<glm::vec3> normals;
	vector<glm::vec3> shared;
	vector<glm::vec3> triangleNormals;

	// generate triangle normals in same order as indices
	for (unsigned int x = 0; x < indices.size(); x++) {
		glm::vec3 e1 = vertices[indices[x].y] - vertices[indices[x].x];
		glm::vec3 e2 = vertices[indices[x].z] - vertices[indices[x].x];

		triangleNormals.push_back(glm::normalize(glm::cross(e1, e2)));
	}

	//For every vertex
	for (unsigned int i = 0; i < vertices.size(); i++){
		shared.clear();
		glm::vec3 norm = { 0, 0, 0 };

		// Find all Shared Normals
		for (unsigned int j = 0; j < indices.size(); j++) {
			if (indices[j].x == i || indices[j].y == i || indices[j].z == i){
				shared.push_back(triangleNormals[j]);
			}
		}

		// Add all shared normals
		for (glm::vec3 s : shared){
			norm += s;
		}
		glm::vec3 normed = glm::normalize(norm);
		cout << "glm::vec3(" << normed.x << ", " << normed.y << ", " << normed.z << ")" << endl;
		normals.push_back(glm::normalize(norm));
	}

	return normals;
}

void Loader::bindTextures(){

	GLuint image = loadDDS("image.bmp");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Loader::loadDDS(const char * imagepath){

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL)
		return 0;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	//ADD MEMORY MANAGEMENT
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);

	return textureID;
}