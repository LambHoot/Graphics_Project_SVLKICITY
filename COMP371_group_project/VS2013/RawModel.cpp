#include "RawModel.h"
#include "Loader.h"

RawModel::RawModel(GLuint vaoID, GLuint elementCount)
{
	this->vaoID = vaoID;
	this->elementCount = elementCount;
}

RawModel::RawModel()
{
	this->vaoID = Loader::createNewVAO();
	this->elementCount = 0;
}

RawModel::~RawModel()
{
}

void RawModel::loadVertices(vector<vec3> vertices){
	GLfloat *ver = new GLfloat[vertices.size() * 3];

	for (unsigned int i = 0; i < vertices.size(); i++){
		ver[3 * i + 0] = vertices[i].x;
		ver[3 * i + 1] = vertices[i].y;
		ver[3 * i + 2] = vertices[i].z;
	}

	Loader::storeDataInAttribList(0, ver, sizeof(ver)*vertices.size() * point_size);
}

void RawModel::loadIndices(vector<vec3> indices){
	GLuint *ind = new GLuint[indices.size() * 3];

	this->elementCount = indices.size() * point_size;

	for (unsigned int i = 0; i < indices.size(); i++){
		ind[3 * i + 0] = indices[i].x;
		ind[3 * i + 1] = indices[i].y;
		ind[3 * i + 2] = indices[i].z;
	}

	Loader::bindIndicesBuffer(ind, sizeof(ind)*indices.size() * point_size);
}

GLuint RawModel::getVAOID(){
	return vaoID;
}

GLuint RawModel::getelementCount(){
	GLuint ret = elementCount;
	return ret;
}