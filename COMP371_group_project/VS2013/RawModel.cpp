#include "RawModel.h"


RawModel::RawModel(GLuint vaoID, GLuint vertexCount)
{
	vaoID = vaoID;
	vertexCount = vertexCount;
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