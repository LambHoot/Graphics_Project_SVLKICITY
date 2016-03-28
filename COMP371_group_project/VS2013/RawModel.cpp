#include "RawModel.h"

RawModel::RawModel(GLuint vaoID, GLuint* indices, GLuint vertexCount)
{
	this->vaoID = vaoID;
	this->indices = indices;
	this->vertexCount = vertexCount;
}


RawModel::~RawModel()
{

}



GLuint RawModel::getVAOID(){
	return vaoID;
}

GLuint* RawModel::getIndices(){
	return indices;
}

GLuint RawModel::getVertexCount(){
	return vertexCount;
}