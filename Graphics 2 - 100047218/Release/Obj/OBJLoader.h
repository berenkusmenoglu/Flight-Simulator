/*************************************************************/
/*                           OBJ                             */
/*                                                           */
/* Purpose: Definitions for CObj a class to load and render  */
/*          OBJ files exported from Alias|Wavefront's Maya   */
/*      Evan Pipho (evan@codershq.com)                       */
/*                                                           */
/*************************************************************/
#ifndef OBJ_H
#define OBJ_H

//-------------------------------------------------------------
//                       INCLUDES                             -
//-------------------------------------------------------------
#include <vector>
#include "../3dStruct/threeDModel.h"


struct ObjMat
{
	//Loaded from the File
	char matName[255];
	float ambientCol[3];
	float diffuseCol[3];
	float specCol[3];
	float d;
	float ns;
	int illum;
	char textureName[255];

	//Created
	int glIndex;
};

//-------------------------------------------------------------
//- SObjFace
//- Indices for each face of the OBJ file
class SObjFace
{
public:

	unsigned int m_uiVertIdx[3];
	unsigned int m_uiNormalIdx[3];
	unsigned int m_uiTexCoordIdx[3];

	int matId;

	SObjFace(unsigned int vert[3], unsigned int text[3], unsigned int norm[3], int newMatId)
	{
		//copy the data;
		m_uiVertIdx[0] = vert[0]-1;
		m_uiVertIdx[1] = vert[1]-1;
		m_uiVertIdx[2] = vert[2]-1;

		m_uiTexCoordIdx[0] = text[0]-1;
		m_uiTexCoordIdx[1] = text[1]-1;
		m_uiTexCoordIdx[2] = text[2]-1;

		m_uiNormalIdx[0] = norm[0]-1;
		m_uiNormalIdx[1] = norm[1]-1;
		m_uiNormalIdx[2] = norm[2]-1;

		matId = newMatId;
	}

	bool operator<(const SObjFace &face2) const  //Defined to sort list
	{
		if (matId < face2.matId) return true;
		else return false;
	}

};


//-------------------------------------------------------------
class OBJLoader
{
public:
	//default constructor
	OBJLoader();

	bool loadModel(char * fileName, ThreeDModel& theResult);

private:
	bool internalLoadModel(char * fileName);
	void readTriangleFaceVertTexNorm(char *line, int matId);
	void readFaceLine(FILE * theFile, int matId);
	bool myFileLoader(char *filename);
	bool myMTLLoader(const char *mainName, const char *filename);
	int lookupMaterial(char *matName);
	void splitFrontString(char * inputString, char * frontString, char * restString);

	
	//The OBJ format does not tell us how many of each thing thing we have
	//so we will use STL vectors in order to have resizable arrays to accomadate
	//the exact number everything
	//an STL vector to hold vertices

	std::vector<Vector3d> m_vVertices;
	//an STL vector to hold vertex normals
	std::vector<Vector3d> m_vNormals;
	//an STL vector to hold the texture coordinates
	std::vector<Vector2d> m_vTexCoords;
	//an STL vector for the faces
	std::vector<SObjFace> m_vFaces;

	std::vector<ObjMat> theMats;

	//Pointer to objects to cut down on operator[] use with std::vector
	Vector3d * m_pVerts;
	Vector3d * m_pNormals;
	Vector2d * m_pTexCoords;
	SObjFace * m_pFaces;

	bool m_bHasTexCoords;
	bool m_bHasNormals;

};


#endif //OBJ_H