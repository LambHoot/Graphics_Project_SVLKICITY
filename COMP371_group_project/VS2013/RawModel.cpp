#include "RawModel.h"

RawModel::RawModel(GLuint vaoID, GLuint vertexCount)
{
	this->vaoID = vaoID;
	this->vertexCount = vertexCount;
}

RawModel::RawModel()
{
	//TODO: Should there be nothing here?
}

RawModel::~RawModel()
{

}

GLuint RawModel::getVAOID(){
	return vaoID;
}

GLuint RawModel::getVertexCount(){
	return vertexCount;
}

bool RawModel::isPointLegal(vec3 point) {
	return true;
}