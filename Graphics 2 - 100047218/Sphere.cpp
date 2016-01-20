#include "Sphere.h"
#include "shaders\Shader.h"

const double PI = 3.14159265358979323846f;  


Sphere::Sphere()
{
	cx = cy = cz = 0.0;
	r = 0.5;
}
Sphere::Sphere(glm::vec3 pos, int rad)
{
	cx = pos.x;
	cy = pos.y;
	cz = pos.z;

	r = rad;
}
void Sphere::setRadius(float rad)
{
	r = rad;
}

void Sphere::setCentre(float x, float y, float z)
{
	cx = x;
	cy = y;
	cz = z;
}

void Sphere::render()
{
	//draw objects
	glBindVertexArray(m_vaoID);		// select VAO

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, numOfTris*3, GL_UNSIGNED_INT, 0);
			
	// Done	
	glBindVertexArray(0); //unbind the vertex array object
	
}

void Sphere::constructGeometry(Shader* myShader, int level)
{
	//create the memory for the sphere
	verts = new float[((level-2)* level+2)*3];
	norms = new float[((level-2)* level+2)*3];
	tInds = new unsigned int[(((level-3)*(level-1) + (level-1)) * 2)*3];

	numOfTris = (((level-3)*(level-1) + (level-1)) * 2);
	numOfVerts = ((level-2)* level+2);

	//populate the arrays
	float theta, phi;  
	int vertCount = 0;
	int i, j, t;  
	
	for( t=0, j=1; j<level-1; j++ )  
	{
		for(      i=0; i<level; i++ )  
		{    
			theta = float(j)/(level-1) * PI;    
			phi   = float(i)/(level-1 ) * PI*2;    
			
			verts[vertCount+t] = ((sinf(theta) * cosf(phi))*r)+cx;
			verts[vertCount+t+1] = (cosf(theta)*r)+cy;
			verts[vertCount+t+2] = ((-sinf(theta) * sinf(phi))*r)+cz;
			
			norms[vertCount+t] = (sinf(theta) * cosf(phi));
			norms[vertCount+t+1] = cosf(theta);
			norms[vertCount+t+2] = -(sinf(theta) * sinf(phi));
		
			t+=3;
		}  
	}
	verts[vertCount+t] = cx;
	verts[vertCount+t+1] = r+cy;
	verts[vertCount+t+2] = cz;

	norms[vertCount+t] = 0;
	norms[vertCount+t+1] = 1;
	norms[vertCount+t+2] = 0;
	t+=3;
	
	verts[vertCount+t] = cx;
	verts[vertCount+t+1] = -r+cy;
	verts[vertCount+t+2] = cz;

	norms[vertCount+t] = 0;
	norms[vertCount+t+1] = -1;
	norms[vertCount+t+2] = 0;
	t+=3;
	
	int vc3 = vertCount / 3;
	int triCount = 0;
	for( t=0, j=0; j<level-3; j++ )  
	{
		for(      i=0; i<level-1; i++ )  
		{    
			tInds[triCount+t] = vc3 + ((j  )*level + i);    t++;
			tInds[triCount+t] = vc3 + ((j+1)*level + i+1);    t++;
			tInds[triCount+t] = vc3 + ((j  )*level + i+1);    t++;
			tInds[triCount+t] = vc3 + ((j  )*level + i ) ;    t++;
			tInds[triCount+t] = vc3 + ((j+1)*level + i ) ;    t++;
			tInds[triCount+t] = vc3 + ((j+1)*level + i+1);  t++;
		}  
	}
	for( i=0; i<level-1; i++ )  
	{    
		tInds[triCount+t] = vc3 + ((level-2)*level);  t++;  
		tInds[triCount+t] = vc3 + (i);    t++;
		tInds[triCount+t] = vc3 + (i+1);    t++;
		tInds[triCount+t] = vc3 + ((level-2)*level+1);    t++;
		tInds[triCount+t] = vc3 + ((level-3)*level + i+1);    t++;
		tInds[triCount+t] = vc3 + ((level-3)*level + i);  t++;
	}
	triCount += t;

	//assign the data to the GPU
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
	glBufferData(GL_ARRAY_BUFFER, numOfVerts*3*sizeof(GLfloat), norms, GL_STATIC_DRAW);
	GLint normalLocation= glGetAttribLocation(myShader->handle(), "in_Normal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(normalLocation);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tInds, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

