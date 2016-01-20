#include "../gl/glew.h"
#include "threeDModel.h"
//#include <gl/glext.h>
#include <math.h>
#include "../texturehandler/texturehandler.h"
#include "../Octree/Octree.h"
#include "../Utilities/IntersectionTests.h"
#include "../shaders/Shader.h"

//extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;

ThreeDModel::ThreeDModel()
{
	fileName = "";
	theVerts = 0;
	theFaceNormals = 0;
	theTexCoords = 0;
	theFaces = 0;
	theMaterials = 0;
	theVertNormals = 0;

	numberOfVertices = 0;
	numberOfFaceNormals =0;
	numberOfTriangles=  0;
	numberOfTexCoords =  0;
	numberOfMatrials = 0;
	numberOfVertNormals = 0;

	octree = NULL;

	indexArray = NULL;	
	
	vertexPositionList = NULL;	
	vertexNormalList = NULL;	
	vertexTexCoordList =  NULL;	
	faceIDsList = NULL;
	glBuffer = NULL;
	vertexColorList = NULL;
}

//void ThreeDModel::killModel()
ThreeDModel::~ThreeDModel()
{
	if(theVerts)	delete [] theVerts;
	if(theFaceNormals)	delete [] theFaceNormals;
	if(theFaces)	delete [] theFaces;
	if(theTexCoords)delete [] theTexCoords;
	if(theMaterials) delete [] theMaterials;
	if(theVertNormals) delete [] theVertNormals;

	theVerts = 0;
	theFaceNormals = 0;
	theTexCoords = 0;
	theFaces = 0;
	theMaterials = 0;
	theVertNormals = 0;

	delete []indexArray;
	indexArray = NULL;	

	delete[] vertexPositionList;
	vertexPositionList = NULL;
	delete[] vertexNormalList;
	vertexNormalList = NULL;
	delete[] vertexTexCoordList;
	vertexTexCoordList =  NULL;
	delete[] faceIDsList;
	faceIDsList = NULL;
	delete[] glBuffer;
	glBuffer = NULL;
	delete[] vertexColorList;
	vertexColorList = NULL;

	startPoints.clear();
	length.clear();	
}


ThreeDModel::ThreeDModel(const ThreeDModel& p)
{
	*this = p;
} 

//Make an assignment opperator
void ThreeDModel::operator=(const ThreeDModel &p)
{	
	fileName = p.fileName;
	numberOfVertices = p.numberOfVertices; 
	numberOfFaceNormals = p.numberOfFaceNormals;   
	numberOfTriangles = p.numberOfTriangles;
	numberOfTexCoords = p.numberOfTexCoords;
	numberOfMatrials = p.numberOfMatrials;
	numberOfVertNormals = p.numberOfVertNormals;

	theBBox = p.theBBox;

	if(p.theVerts != NULL)
	{
		theVerts = new Vector3d[numberOfVertices];
		theFaceNormals = new Vector3d[numberOfFaceNormals];
		theTexCoords = new Vector2d[numberOfTexCoords];
		theFaces = new aFace[numberOfTriangles];
		theMaterials = new aMaterial[numberOfMatrials];
		theVertNormals = new Vector3d[numberOfVertNormals];

		//Copy over Data
		memcpy(theVerts, p.theVerts, sizeof(Vector3d) * numberOfVertices);
		memcpy(theFaceNormals, p.theFaceNormals, sizeof(Vector3d) * numberOfFaceNormals);
		memcpy(theTexCoords, p.theTexCoords, sizeof(Vector2d) * numberOfTexCoords);
		memcpy(theFaces,p.theFaces, sizeof(aFace) * numberOfTriangles);
		memcpy(theMaterials, p.theMaterials, sizeof(aMaterial) * numberOfMatrials);
		memcpy(theVertNormals, p.theVertNormals, sizeof(Vector3d) * numberOfVertNormals);
	}
	else
	{
		theVerts = NULL;
		theFaceNormals = NULL;
		theTexCoords = NULL;
		theFaces = NULL;
		theMaterials = NULL;
		theVertNormals = NULL;
	}

	indexArray = NULL;
	length.clear();
	for(unsigned int i=0;i<p.length.size();i++)
	{
		length.push_back(p.length[i]);
	}

	if(p.vertexPositionList != NULL)
	{
		vertexPositionList = new GLfloat[numberOfTriangles*3*3];
		vertexNormalList = new GLfloat[numberOfTriangles*3*3];
		vertexTexCoordList = new GLfloat[numberOfTriangles*3*2];
		faceIDsList=new GLuint[numberOfTriangles*3];

		memcpy(vertexPositionList,p.vertexPositionList,sizeof(GLfloat)*numberOfTriangles*3*3);
		memcpy(vertexNormalList,p.vertexNormalList,sizeof(GLfloat)*numberOfTriangles*3*3);
		memcpy(vertexTexCoordList,p.vertexTexCoordList,sizeof(GLfloat)*numberOfTriangles*3*2);
		memcpy(faceIDsList,p.faceIDsList,sizeof(GLuint)*numberOfTriangles*3);
	}
	else
	{
		vertexPositionList = NULL;
		vertexNormalList = NULL;
		vertexTexCoordList = NULL;
		faceIDsList = NULL;
	}

	if(p.vertexColorList != NULL)
	{
		std::cout << " set up color " << std::endl;
		vertexColorList = new GLfloat[numberOfTriangles*3*3];
		memcpy(vertexColorList,p.vertexColorList,sizeof(GLfloat)*numberOfTriangles*3*3);
	}
	else
	{
		vertexColorList = NULL;
	}

	glBuffer = NULL;
}

void ThreeDModel::constructOctree()
{
	double minX,minY,minZ,maxX,maxY,maxZ;
	calcBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
	//cout << minX << " " << minY << " " << minZ << endl;
	//cout << maxX << " " << maxY << " " << maxZ << endl;

	octree = new Octree();
	octree->start(0, minX, minY, minZ, maxX, maxY, maxZ, this);

	vector<Octree*> stackOctree;
	stackOctree.push_back(octree);

	while(stackOctree.size() > 0)
	{
		Octree* oct = stackOctree[0];
		oct->CreateChildren(stackOctree, this);

		stackOctree.erase(stackOctree.begin());
	}
}

void ThreeDModel::calcVertNormalsUsingOctree()
{
	if(theVertNormals != NULL) //delete any previously created vertex normals
	{
		delete [] theVertNormals;
	}
	
	if(octree == NULL) // construct the octree if it hasn't been created.
	{
		constructOctree();
	}

	theVertNormals = new Vector3d[numberOfVertices];
	numberOfVertNormals = numberOfVertices;

	//calculate the vertex normals using the octree
	octree->processVerticesByLeaf(this);
}

void ThreeDModel::drawBoundingBox(Shader* myShader)
{
	if(octree!=NULL)
	{
		//octree->drawBoundingBox(myShader);
		octree->drawAllBoxes(myShader);
	}
}

void ThreeDModel::drawOctreeLeaves(Shader* myShader)
{
	if(octree != NULL)
	{
		octree->drawBoxesAtLeaves(myShader);
	}
}

void ThreeDModel::calcCentrePoint()
{
	if(theVerts == NULL)
	{
		float maxX, maxY, maxZ;
		float minX, minY, minZ;

		minX = minY = minZ = +100000.0f;
		maxX = maxY = maxZ = -100000.0f;


		//for all the verticies (can ignore everthing else)

		for (int count = 0; count < numberOfTriangles*3*3; count+=3)
		{
			//if ((theVerts[count].x > -1000) && (theVerts[count].y > -1000) && (theVerts[count].z > -1000))
			{
				if (vertexPositionList[count] > maxX) maxX = vertexPositionList[count];
				if (vertexPositionList[count+1] > maxY) maxY = vertexPositionList[count+1];
				if (vertexPositionList[count+2] > maxZ) maxZ = vertexPositionList[count+2];

				if (vertexPositionList[count] < minX) minX = vertexPositionList[count];
				if (vertexPositionList[count+1] < minY) minY = vertexPositionList[count+1];
				if (vertexPositionList[count+2] < minZ) minZ = vertexPositionList[count+2];
			}
		}

		theBBox.boxWidthX = float(maxX-minX);
		theBBox.boxWidthY = float(maxY-minY);
		theBBox.boxWidthZ = float(maxZ-minZ);

		theBBox.centrePoint = Vector3d(minX+(theBBox.boxWidthX/2.0f),
			minY+(theBBox.boxWidthY/2.0f),
			minZ+(theBBox.boxWidthZ/2.0f));
		return;
	}

	float maxX, maxY, maxZ;
	float minX, minY, minZ;

	minX = minY = minZ = +100000.0f;
	maxX = maxY = maxZ = -100000.0f;


	//for all the verticies (can ignore everthing else)
	
	for (int count = 0; count < numberOfVertices; count++)
	{
		//if ((theVerts[count].x > -1000) && (theVerts[count].y > -1000) && (theVerts[count].z > -1000))
		{
			if (theVerts[count].x > maxX) maxX = theVerts[count].x;
			if (theVerts[count].y > maxY) maxY = theVerts[count].y;
			if (theVerts[count].z > maxZ) maxZ = theVerts[count].z;
			
			if (theVerts[count].x < minX) minX = theVerts[count].x;
			if (theVerts[count].y < minY) minY = theVerts[count].y;
			if (theVerts[count].z < minZ) minZ = theVerts[count].z;
		}
	}
	
	theBBox.boxWidthX = float(maxX-minX);
	theBBox.boxWidthY = float(maxY-minY);
	theBBox.boxWidthZ = float(maxZ-minZ);

	theBBox.centrePoint = Vector3d(minX+(theBBox.boxWidthX/2.0f),
								   minY+(theBBox.boxWidthY/2.0f),
								   minZ+(theBBox.boxWidthZ/2.0f));
	
	
}

int ThreeDModel::getOctreePrimitiveListSize()
{
	return numberOfTriangles;
}

int ThreeDModel::getOctreeVertexListSize()
{
	return numberOfVertices;
}

bool ThreeDModel::isVertexIntersectingOctreeCell(double min[3], double max[3], int VertIndex)
{
	if(theVerts[VertIndex].x >= min[0] && theVerts[VertIndex].x <= max[0] &&
		theVerts[VertIndex].y >= min[1] && theVerts[VertIndex].y <= max[1] &&
		theVerts[VertIndex].z >= min[2] && theVerts[VertIndex].z <= max[2])
	{
		return true;
	}
	return false;
}

bool ThreeDModel::isPrimitiveIntersectingOctreeCell(double boxCenter[3], double boxHalfSize[3], int PrimIndex)
{
	double triVerts[3][3];

	triVerts[0][0] = theVerts[theFaces[PrimIndex].thePoints[0]].x;
	triVerts[0][1] = theVerts[theFaces[PrimIndex].thePoints[0]].y;
	triVerts[0][2] = theVerts[theFaces[PrimIndex].thePoints[0]].z;

	triVerts[1][0] = theVerts[theFaces[PrimIndex].thePoints[1]].x;
	triVerts[1][1] = theVerts[theFaces[PrimIndex].thePoints[1]].y;
	triVerts[1][2] = theVerts[theFaces[PrimIndex].thePoints[1]].z;

	triVerts[2][0] = theVerts[theFaces[PrimIndex].thePoints[2]].x;
	triVerts[2][1] = theVerts[theFaces[PrimIndex].thePoints[2]].y;
	triVerts[2][2] = theVerts[theFaces[PrimIndex].thePoints[2]].z;

	if(IntersectionTests::triBoxOverlap(boxCenter, boxHalfSize, triVerts)==1)
	{
		return true;
	}
	return false;
}

void ThreeDModel::calcBoundingBox(double& minX, double& minY, double& minZ, double& maxX, double& maxY, double& maxZ)
{	
	maxX = minX = theVerts[0].x;
	maxY = minY = theVerts[0].y;
	maxZ = minZ = theVerts[0].z;
	double x,y,z;
	for(int i=1; i<numberOfVertices; i++)
	{
		
		x = theVerts[i].x;
		y = theVerts[i].y;
		z = theVerts[i].z;

		if(x < minX)
			minX = x;
		if(x > maxX)
			maxX = x;
		if(y < minY)
			minY = y;
		if(y > maxY)
			maxY = y;
		if(z < minZ)
			minZ = z;
		if(z > maxZ)
			maxZ = z;
	}
}

void ThreeDModel::calcFaceNormals()
{
	if(theVerts == NULL)
	{
	}
	else
	{
		for (int i = 0; i < numberOfTriangles; i++)
		{
			Vector3d edge1 = theVerts[theFaces[i].thePoints[0]] - theVerts[theFaces[i].thePoints[1]];
			Vector3d edge2 = theVerts[theFaces[i].thePoints[0]] - theVerts[theFaces[i].thePoints[2]];

			Vector3d normal =   edge1 * edge2;
			normal.normalize();

			theFaceNormals[i]  = normal;

			theFaces[i].theFaceNormal = i;
		}
	}
}


void ThreeDModel::calcFakeVertNormals()
{
	if(theVerts != NULL)
	{
		//If they exist - delete them
		if (theVertNormals) delete [] theVertNormals;


		//Setup the Arrays to hold them
		theVertNormals = new Vector3d[numberOfVertices];
		numberOfVertNormals = numberOfVertices;

		for (int count = 0; count < numberOfVertNormals; count ++)
		{
			for (int count2 = 0; count2 < numberOfTriangles; count2++)
			{
				if (count == theFaces[count2].thePoints[0] ||
					count == theFaces[count2].thePoints[1] ||
					count == theFaces[count2].thePoints[2])
				{
					theVertNormals[count] = theFaceNormals[theFaces[count2].theFaceNormal];
					break;
				}
			}
		}
	}
}

void ThreeDModel::calcVertNormals()
{
	if(theVerts != NULL)
	{
		Vector3d * newNormals = new Vector3d[numberOfVertices];

		for (int i = 0; i < numberOfVertices; i++) //For all the verts
		{
			Vector3d theNormal(0,0,0);
			for (int j = 0; j < numberOfTriangles; j++)		 //GO through all the faces
			{
				for (int k = 0; k < 3; k++)
				{
					if (theFaces[j].thePoints[k] == i) //if index equals current index
					{	
						theNormal = theNormal + theFaceNormals[theFaces[j].theFaceNormal];
					}
				}
			}
			theNormal.normalize();
			newNormals[i] = theNormal;
		}

		theVertNormals = new Vector3d[numberOfVertices];
		numberOfVertNormals = numberOfVertices;


		for (int count = 0; count < numberOfVertNormals; count ++)
		{
			theVertNormals[count] = newNormals[count];
		}
	}
}

void ThreeDModel::calcVertNormals(int* VertList, int VertListSize, int* TriList, int TriListSize)
{
	if(theVerts != NULL)
	{
		for (int i = 0; i < VertListSize; i++) //For all the verts
		{
			Vector3d theNormal(0,0,0);
			for (int j = 0; j < TriListSize; j++)		 //GO through all the faces
			{
				for (int k = 0; k < 3; k++)
				{
					if (theFaces[TriList[j]].thePoints[k] == VertList[i]) //if index equals current index
					{	
						theNormal = theNormal + theFaceNormals[theFaces[TriList[j]].theFaceNormal];
					}
				}
			}
			theNormal.normalize();

			theVertNormals[VertList[i]] = theNormal;
		}
	}
}

void ThreeDModel::calcSidePointObject(Vector3d &thePoint, float w, aFace &theObject)
{

}

void ThreeDModel::centreOnZero()
{
	//Subtract centre point from each point
	if(theVerts == NULL)
	{
		for (int count = 0; count < numberOfTriangles*3*3; count+=3)
		{
			vertexPositionList[count] = vertexPositionList[count] - theBBox.centrePoint.x;
			vertexPositionList[count+1] = vertexPositionList[count+1] - theBBox.centrePoint.y;
			vertexPositionList[count+2] = vertexPositionList[count+2] - theBBox.centrePoint.z;
		}
	}
	else
	{
		for (int count = 0; count < numberOfVertices; count++)
		{
			theVerts[count] = theVerts[count] - theBBox.centrePoint;
		}
	}
	theBBox.centrePoint = Vector3d(0,0,0);
}

void ThreeDModel::scale(float scaleAmount)
{
	if(theVerts == NULL)
	{
		for (int count = 0; count < numberOfTriangles*3*3; count++)
		{
			vertexPositionList[count] = vertexPositionList[count] * scaleAmount;
		}
	}
	else
	{
		for (int count = 0; count < numberOfVertices; count++)
		{
			theVerts[count] = theVerts[count] * scaleAmount;
		}
	}
}

void ThreeDModel::translate(Vector3d transVec)
{
	if(theVerts == NULL)
	{
		for (int count = 0; count < numberOfTriangles*3*3; count+=3)
		{
			//	Vector3d bob = theVerts[count];
			vertexPositionList[count] = vertexPositionList[count] + transVec.x;
			vertexPositionList[count+1] = vertexPositionList[count+1] + transVec.y;
			vertexPositionList[count+2] = vertexPositionList[count+2] + transVec.z;
		}
	}
	else
	{
		for (int count = 0; count < numberOfVertices; count++)
		{
			//	Vector3d bob = theVerts[count];
			theVerts[count] = theVerts[count] + transVec;
		}
	}
}


void ThreeDModel::adjustBoundingBox()
{
	float maxdisplace = 0;
	if(theVerts == NULL)
	{
		for (int count = 0; count < numberOfTriangles*3*3; count+=3)
		{
			if ((vertexPositionList[count] > -1000) && (vertexPositionList[count+1] > -1000) && (vertexPositionList[count+2] > -1000))
			{
				Vector2d tempVec(vertexPositionList[count]-theBBox.centrePoint.x, vertexPositionList[count+2]-theBBox.centrePoint.z);
				float tempSize = sqrt(tempVec.x * tempVec.x + tempVec.y * tempVec.y);
				if (tempSize > maxdisplace)
				{
					maxdisplace = tempSize;
				}
			}
		}
	}
	else
	{
		for (int count = 0; count < numberOfVertices; count++)
		{
			if ((theVerts[count].x > -1000) && (theVerts[count].y > -1000) && (theVerts[count].z > -1000))
			{
				Vector2d tempVec(theVerts[count].x-theBBox.centrePoint.x, theVerts[count].z-theBBox.centrePoint.z);
				float tempSize = sqrt(tempVec.x * tempVec.x + tempVec.y * tempVec.y);
				if (tempSize > maxdisplace)
				{
					maxdisplace = tempSize;
				}
			}
		}
	}

	theBBox.boxWidthX = maxdisplace * 2.0f;
	theBBox.boxWidthZ = maxdisplace * 2.0f;
}


void ThreeDModel::initVBO(Shader* myShader)
{
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	int numOfMaterials = length.size()/3;
	std::cout << " initVBO " << numOfMaterials << std::endl;

	int sizeOfGLBuffer = 4+numOfMaterials;
	glBuffer = new GLuint[sizeOfGLBuffer];
	glGenBuffers(sizeOfGLBuffer, glBuffer);

	std::cout << " make vertexPositionList, numOfTriangles " << numberOfTriangles << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, glBuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, numberOfTriangles*3*3*sizeof(GLfloat), vertexPositionList, GL_STATIC_DRAW);
	GLint vertexLocation= glGetAttribLocation(myShader->handle(), "in_Position");
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(vertexLocation);


	std::cout << " make vertexNormalList " << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, glBuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, numberOfTriangles*3*3*sizeof(GLfloat), vertexNormalList, GL_STATIC_DRAW);
		GLint normalLocation = glGetAttribLocation(myShader->handle(), "in_Normal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(normalLocation);


	std::cout << " make vertexTexCoordList " << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, glBuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, numberOfTriangles*3*2*sizeof(GLfloat), vertexTexCoordList, GL_STATIC_DRAW);
	GLint texCoordLocation = glGetAttribLocation(myShader->handle(), "in_TexCoord");
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0,0);
	glEnableVertexAttribArray(texCoordLocation);


	if(vertexColorList != NULL)
	{
		std::cout << " make vertexColorList " << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER, glBuffer[3]);
		glBufferData(GL_ARRAY_BUFFER, numberOfTriangles*3*3*sizeof(GLfloat), vertexColorList, GL_STATIC_DRAW);
	}

	
	std::cout << " make faceIDsList " << std::endl;
	for(unsigned int i=0;i<length.size();i+=3)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer[4+(i/3)]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (length[i+1]-length[i]+3)*sizeof(GLuint), faceIDsList+(length[i]), GL_STATIC_DRAW);
	}

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void ThreeDModel::initDrawElements()
{
	if(vertexPositionList == NULL)
	{
	vertexPositionList = new GLfloat[numberOfTriangles*3*3];
	vertexNormalList = new GLfloat[numberOfTriangles*3*3];
	vertexTexCoordList = new GLfloat[numberOfTriangles*3*2];

	faceIDsList=new GLuint[numberOfTriangles*3];
	for(int i=0; i<numberOfTriangles*3; i+=3)
	{
		faceIDsList[i]=i;
		faceIDsList[i+1]=(i+1);
		faceIDsList[i+2]=(i+2);
	}
	

	length.clear();
	unsigned int polyCount = 0;
	unsigned int polyCounter = 0;
	std::cout << " numberOftriangles " << numberOfTriangles << std::endl;
	int matID = theMaterials[theFaces[0].materialId].textureID;
	for(int x = 0; x < numberOfTriangles; x++)
	{
		if (matID != theMaterials[theFaces[x].materialId].textureID)
		{
			length.push_back(polyCount*3);
			length.push_back((polyCount+polyCounter-1)*3);		
			length.push_back(matID);
			matID = theMaterials[theFaces[x].materialId].textureID;			
			//polyCount = 0;
			polyCount = polyCount+polyCounter;
			polyCounter = 1;			
		}
		else
		{
			//polyCount++;
			polyCounter++;
		}
		//polyCount++;
	}
	length.push_back(polyCount*3);//*3
	length.push_back((polyCount+polyCounter-1)*3);//*3
	//length.push_back(numberOfTriangles*3);
	length.push_back(matID);
	std::cout << " number of different textures: " << length.size()/3 << std::endl;	
	//length.push_back(0);
	//length.push_back(numberOfTriangles*3);

	for(int triIndex=0,i=0,j=0; triIndex<numberOfTriangles; triIndex++,i+=3, j+=2)
	{
		aFace temp = theFaces[triIndex];		
	
		unsigned int vert1 = temp.thePoints[0];
		unsigned int vert2 = temp.thePoints[1];
		unsigned int vert3 = temp.thePoints[2];
		
		unsigned int norm1 = temp.theFaceNormal;

		unsigned int tex1 = temp.theTexCoord[0];
		unsigned int tex2 = temp.theTexCoord[1];
		unsigned int tex3 = temp.theTexCoord[2];

		vertexPositionList[i]=theVerts[vert1].x;
		vertexPositionList[i+1]=theVerts[vert1].y;
		vertexPositionList[i+2]=theVerts[vert1].z;
		
		if(numberOfVertNormals > 0)
		{
			vertexNormalList[i]=theVertNormals[vert1].x;
			vertexNormalList[i+1]=theVertNormals[vert1].y;
			vertexNormalList[i+2]=theVertNormals[vert1].z;
		}
		else if(numberOfFaceNormals > 0)
		{
			vertexNormalList[i]=theFaceNormals[norm1].x;
			vertexNormalList[i+1]=theFaceNormals[norm1].y;
			vertexNormalList[i+2]=theFaceNormals[norm1].z;
		}
		else
		{
			vertexNormalList[i]=0;
			vertexNormalList[i+1]=0;
			vertexNormalList[i+2]=0;
		}
				
		vertexTexCoordList[j]=theTexCoords[tex1].x;;
		vertexTexCoordList[j+1]=theTexCoords[tex1].y;

		i+=3;
		j+=2;

		vertexPositionList[i]=theVerts[vert2].x;
		vertexPositionList[i+1]=theVerts[vert2].y;
		vertexPositionList[i+2]=theVerts[vert2].z;
		
		if(numberOfVertNormals > 0)
		{
			vertexNormalList[i]=theVertNormals[vert2].x;
			vertexNormalList[i+1]=theVertNormals[vert2].y;
			vertexNormalList[i+2]=theVertNormals[vert2].z;
		}
		else if(numberOfFaceNormals > 0)
		{
			vertexNormalList[i]=theFaceNormals[norm1].x;
			vertexNormalList[i+1]=theFaceNormals[norm1].y;
			vertexNormalList[i+2]=theFaceNormals[norm1].z;
		}
		else
		{
			vertexNormalList[i]=0;
			vertexNormalList[i+1]=0;
			vertexNormalList[i+2]=0;
		}
				
		vertexTexCoordList[j]=theTexCoords[tex2].x;;
		vertexTexCoordList[j+1]=theTexCoords[tex2].y;

		i+=3;
		j+=2;

		vertexPositionList[i]=theVerts[vert3].x;
		vertexPositionList[i+1]=theVerts[vert3].y;
		vertexPositionList[i+2]=theVerts[vert3].z;
		
		if(numberOfVertNormals > 0)
		{
			vertexNormalList[i]=theVertNormals[vert3].x;
			vertexNormalList[i+1]=theVertNormals[vert3].y;
			vertexNormalList[i+2]=theVertNormals[vert3].z;
		}
		else if(numberOfFaceNormals > 0)
		{
			vertexNormalList[i]=theFaceNormals[norm1].x;
			vertexNormalList[i+1]=theFaceNormals[norm1].y;
			vertexNormalList[i+2]=theFaceNormals[norm1].z;
		}
		else
		{
			vertexNormalList[i]=0;
			vertexNormalList[i+1]=0;
			vertexNormalList[i+2]=0;
		}
				
		vertexTexCoordList[j]=theTexCoords[tex3].x;;
		vertexTexCoordList[j+1]=theTexCoords[tex3].y;
	}	

	
	}
}

void ThreeDModel::drawElementsUsingVBO(Shader* myShader)
{
	glBindVertexArray(m_vaoID);	
		
	glUniform1i(glGetUniformLocation(myShader->handle(), "DiffuseMap"), 0);

	for(unsigned int i=0;i<length.size();i+=3)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, length[i+2]);// count]/3	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		//std::cout << " length " << length[i] << " " << length[i+1] << " " << length[i+2] << std::endl;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer[4+(i/3)]);
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, (length[i+1]-length[i]+3), GL_UNSIGNED_INT, 0);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
	
	glBindVertexArray(0);	
}

static int   sortByMat( const void *tpi, const void *tpj )
{
	aFace* pi, *pj;
	pi = (aFace*)tpi;
	pj = (aFace*)tpj;

	if(pi->materialId < pj->materialId)
	{
		return -1;
	}
	else if(pi->materialId > pj->materialId)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void ThreeDModel::sortFacesOnMaterial()
{
	qsort(theFaces,numberOfTriangles,sizeof(aFace),sortByMat);		
}
void ThreeDModel::deleteVertexFaceData()
{
	delete [] theVerts;
	delete [] theFaceNormals;
	delete [] theFaces;
	delete [] theTexCoords;
	delete [] theMaterials;
	delete [] theVertNormals;

	theVerts = 0;
	theFaceNormals = 0;
	theTexCoords = 0;
	theFaces = 0;
	theMaterials = 0;
	theVertNormals = 0;
}