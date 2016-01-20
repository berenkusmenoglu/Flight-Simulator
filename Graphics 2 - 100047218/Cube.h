
#include "gl\glew.h"

class Shader;

const int NumberOfVertexCoords = 24;
const int NumberOfTriangleIndices = 36;

class Cube
{
private:
	float dim;
	unsigned int m_vaoID;		   
	unsigned int m_vboID[2];		
	GLuint ibo;                    

	static int numOfVerts;
	static int numOfTris;

	float verts[NumberOfVertexCoords];
	float cols[NumberOfVertexCoords];
	unsigned int tris[NumberOfTriangleIndices];
public:
	Cube();
	void setDim(float d);
	void constructGeometry(Shader* myShader);
	void render(int texID);
};