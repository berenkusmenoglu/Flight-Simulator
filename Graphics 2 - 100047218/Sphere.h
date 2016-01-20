
#include "gl\glew.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
class Shader;

class Sphere
{
private:
	unsigned int m_vaoID;		 
	unsigned int m_vboID[3];	
	GLuint ibo;                  

	int numOfVerts;
	int numOfTris;

	float* verts;
	float* norms;
	unsigned int* tInds;

public:
	Sphere();
	Sphere(glm::vec3 pos, int rad);
	float cx, cy, cz; 
	float r;		
	void constructGeometry(Shader* myShader, int level);
	void render();
	void setRadius(float rad);
	void setCentre(float x, float y, float z);
};