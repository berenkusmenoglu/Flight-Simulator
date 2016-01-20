#include "Box.h"
#include "shaders\Shader.h"

int Box::numOfTris = 12;
int Box::numOfVerts = 8;

Box::Box()
{
	dim = 1.0;
}

void Box::render()
{
	//draw objects
	glBindVertexArray(m_vaoID);		// select VAO

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, numOfTris*3, GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
	// Done
	
	glBindVertexArray(0); //unbind the vertex array object
	
}

void Box::constructGeometry(Shader* myShader, float minx, float miny, float minz, float maxx, float maxy, float maxz)
{
	// First simple object
	verts[0] = minx;   verts[ 1] = miny;  verts[ 2] = minz;
	verts[3] = minx;   verts[ 4] =  maxy;  verts[ 5] = minz;
	verts[6] =  maxx;   verts[ 7] =  maxy;  verts[ 8] = minz;
	verts[9] =  maxx;   verts[10] = miny;  verts[11] = minz;

	verts[12] = minx;   verts[13] = miny;  verts[14] = maxz;
	verts[15] = minx;   verts[16] =  maxy;  verts[17] = maxz;
	verts[18] =  maxx;   verts[19] =  maxy;  verts[20] = maxz;
	verts[21] =  maxx;   verts[22] = miny;  verts[23] = maxz;

	cols[0] = 0.0;   cols[ 1] = 0.0;  cols[ 2] = 0.0;
	cols[3] = 0.0;   cols[ 4] = 1.0;  cols[ 5] = 0.0;
	cols[6] = 0.0;   cols[ 7] = 0.0;  cols[ 8] = 1.0;
	cols[9] = 1.0;   cols[10] = 1.0;  cols[11] = 1.0;

	cols[12] = 1.0;   cols[13] = 0.0;  cols[14] = 0.0;
	cols[15] = 0.0;   cols[16] = 1.0;  cols[17] = 0.0;
	cols[18] = 0.0;   cols[19] = 0.0;  cols[20] = 1.0;
	cols[21] = 1.0;   cols[22] = 1.0;  cols[23] = 0.0;
	
	tris[0]=0; tris[1]=1; tris[2]=2;
	tris[3]=0; tris[4]=2; tris[5]=3;
	tris[6]=4; tris[7]=6; tris[8]=5;
	tris[9]=4; tris[10]=7; tris[11]=6;
	tris[12]=1; tris[13]=5; tris[14]=6;
	tris[15]=1; tris[16]=6; tris[17]=2;
	tris[18]=0; tris[19]=7; tris[20]=4;
	tris[21]=0; tris[22]=3; tris[23]=7;
	tris[24]=0; tris[25]=5; tris[26]=1;
	tris[27]=0; tris[28]=4; tris[29]=5;
	tris[30]=3; tris[31]=2; tris[32]=7;
	tris[33]=2; tris[34]=6; tris[35]=7;
		
	// VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);
	
	glGenBuffers(2, m_vboID);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, numOfVerts*3*sizeof(GLfloat), verts, GL_STATIC_DRAW);
	GLint vertexLocation= glGetAttribLocation(myShader->handle(), "in_Position");
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(vertexLocation);

	
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, numOfVerts*3*sizeof(GLfloat), cols, GL_STATIC_DRAW);
	GLint colorLocation= glGetAttribLocation(myShader->handle(), "in_Color");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(colorLocation);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tris, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
