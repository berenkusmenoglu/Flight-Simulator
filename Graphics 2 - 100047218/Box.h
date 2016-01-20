#ifndef _BOX_H
#define _BOX_H

#include "gl\glew.h"

class Shader;

const int NumberOfVertexCoords = 24;
const int NumberOfTriangleIndices = 36;


class Box
{
private:
	float dim;
	unsigned int m_vaoID;		    // vertex array object
	unsigned int m_vboID[2];		// two VBOs - used for colours and vertex data
	GLuint ibo;                     //identifier for the triangle indices

	static int numOfVerts;
	static int numOfTris;

	float verts[NumberOfVertexCoords];
	float cols[NumberOfVertexCoords];
	unsigned int tris[NumberOfTriangleIndices];
public:
	Box();

	void constructGeometry(Shader* myShader, float minx, float miny, float minz, float maxx, float maxy, float maxz);
	void render();
};

#endif _BOX_H