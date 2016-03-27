#include "RawModel.h"

RawModel::RawModel(GLuint vaoID, GLuint vertexCount)
{
	this->vaoID = vaoID;
	this->vertexCount = vertexCount;
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