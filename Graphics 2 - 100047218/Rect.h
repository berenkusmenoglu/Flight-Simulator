#pragma once

#include "gl\glew.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"


class Shader;

class Rect
{

private:
	unsigned int m_vaoID;		   
	unsigned int m_vboID[3];		
	GLuint ibo;                    
	
	static int numOfVerts;
	static int numOfTris;

	float verts[24];
	float cols[24];
	unsigned int tris[36];

public:
	Rect();
	Rect(glm::vec3 pos, int dim);
	Rect(glm::vec3 pos, int width, int height, int depth);
	float cx, cy, cz; 
	int dim;
	int width, height, depth;

	void Rect::constructGeometry(Shader* myShader);
	void render();

};

