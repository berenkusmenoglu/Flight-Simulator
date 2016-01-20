#include "OBJLoader.h"
#include <iostream>
#include "../TextureHandler/TextureHandler.h"
#include <sstream>

OBJLoader::OBJLoader()
{
	m_pVerts = 0;
	m_pTexCoords = 0;
	m_pNormals = 0;
	m_pFaces = 0;
	m_bHasTexCoords = false;
	m_bHasNormals = false;
}

bool OBJLoader::loadModel(char * fileName, ThreeDModel& theResult)
{	
	m_vVertices.clear();
	m_vNormals.clear();
	m_vTexCoords.clear();
	m_vFaces.clear();
	theMats.clear();

	if(!myFileLoader(fileName))
	{
		return false;
	}

	theResult.numberOfVertices = (int)m_vVertices.size();
	theResult.numberOfTriangles = (int)m_vFaces.size();
	theResult.numberOfTexCoords = (int) m_vTexCoords.size();
	theResult.numberOfVertNormals = (int)m_vNormals.size();
	theResult.numberOfMatrials = (int)theMats.size();
	theResult.numberOfFaceNormals = (int)m_vFaces.size();

	//Output models Stats	
	std::cout << "Number of Vertices " << theResult.numberOfVertices << std::endl;
	std::cout << "Number of Triangles " << theResult.numberOfTriangles << std::endl;
	std::cout << "Number of Vert Normals " << theResult.numberOfVertNormals << std::endl;
	std::cout << "Number of Face Normals " << theResult.numberOfFaceNormals << std::endl;
	std::cout << "Number of TexCoords " << theResult.numberOfTexCoords << std::endl;
	std::cout << "Number of Materials " << theResult.numberOfMatrials << std::endl;


	
	//Copy over the airobject.model
	theResult.theFaceNormals = new Vector3d[theResult.numberOfFaceNormals];
	theResult.theFaces = new aFace[theResult.numberOfTriangles];
	for (int i = 0; i < theResult.numberOfTriangles; i++)
	{
		aFace* currentFace = &theResult.theFaces[i];
		
		currentFace->thePoints[0] =  m_vFaces[i].m_uiVertIdx[0];
		currentFace->thePoints[1] =  m_vFaces[i].m_uiVertIdx[1];
		currentFace->thePoints[2] =  m_vFaces[i].m_uiVertIdx[2];
	
/*		currentFace->theNormals[0] =  m_vFaces[i].m_uiNormalIdx[0];
		currentFace->theNormals[1] =  m_vFaces[i].m_uiNormalIdx[1];
		currentFace->theNormals[2] =  m_vFaces[i].m_uiNormalIdx[2]; */

		currentFace->theTexCoord[0] =  m_vFaces[i].m_uiTexCoordIdx[0];
		currentFace->theTexCoord[1] =  m_vFaces[i].m_uiTexCoordIdx[1];
		currentFace->theTexCoord[2] =  m_vFaces[i].m_uiTexCoordIdx[2]; 

		currentFace->materialId = m_vFaces[i].matId;
	}



	//Load the First Frame into the threeDModel
	theResult.theVerts = new Vector3d[theResult.numberOfVertices];
	for (int i = 0; i < theResult.numberOfVertices; i++)
	{	
		theResult.theVerts[i] = m_vVertices[i];
	}


	//Load the Normals
	theResult.theVertNormals = new Vector3d[theResult.numberOfVertNormals];
	for (int i = 0; i < theResult.numberOfVertNormals; i++)
	{	
		theResult.theVertNormals[i] = m_vNormals[i];
	//	theResult.theNormals[i].normalize();
	}


	theResult.theTexCoords = new Vector2d[theResult.numberOfTexCoords];
	for (int i = 0; i < theResult.numberOfTexCoords; i++)
	{	
		theResult.theTexCoords[i] = m_vTexCoords[i];
	}


	theResult.theMaterials = new aMaterial[theResult.numberOfMatrials];
	for (int i = 0; i < theResult.numberOfMatrials; i++)
	{	
		aMaterial temp;
		ObjMat current = theMats[i];

		temp.ambient[0] = current.ambientCol[0];
		temp.ambient[1] = current.ambientCol[1];
		temp.ambient[2] = current.ambientCol[2];

		temp.diffuse[0] = current.diffuseCol[0];
		temp.diffuse[1] = current.diffuseCol[1];
		temp.diffuse[2] = current.diffuseCol[2];

		temp.specular[0] = current.specCol[0];
		temp.specular[1] = current.specCol[1];
		temp.specular[2] = current.specCol[2];

		temp.textureID = current.glIndex;

		strcpy_s(temp.matName,sizeof(char)*255, current.matName);
		strcpy_s(temp.fileName, sizeof(char)*255, current.textureName);

		theResult.theMaterials[i] = temp;
	}
	
	theResult.calcFaceNormals();

	theResult.sortFacesOnMaterial();
	
	return true;
}

void OBJLoader::splitFrontString(char * inputString, char * frontString, char * restString)
{
	sscanf_s(inputString,"%s", frontString);
	
	//Get length of frontString
	
	int x = 0;
	while (frontString[x] != '\0') x++;
	if (inputString[0] == ' ') x++;
	

	int y = 0;
	while (inputString[y+x+1] != '\0')
	{
		restString[y] = inputString[y+x+1];
		y++;
	}
	restString[y] = '\0';
	

}


void OBJLoader::readTriangleFaceVertTexNorm(char *line, int matId)
{
	unsigned int m_uiVertIdx[3];
	unsigned int m_uiTexCoordIdx[3];
	unsigned int m_uiNormalIdx[3];

	for (int x = 0; x< 3; x++)
	{
		char currentArg[255];
		char remStr[255]; 

		splitFrontString(line, currentArg, remStr); 

		sscanf_s(currentArg,"%i/%i/%i", &m_uiVertIdx[x],&m_uiTexCoordIdx[x], &m_uiNormalIdx[x]);
		memcpy_s(line,sizeof(char)*255, remStr,255);
	}

	SObjFace newFace(m_uiVertIdx, m_uiTexCoordIdx,  m_uiNormalIdx, matId);

	m_vFaces.push_back(newFace);
}

void OBJLoader::readFaceLine(FILE * theFile, int matId)
{
	char line[255];
	fgets(line, 255, theFile); //read in the whole line				

	readTriangleFaceVertTexNorm(line, matId);
}	

bool OBJLoader::myMTLLoader(const char *mainName, const char *filename)
{

	char line[255];

	std::string fullPath(mainName);

	int whereDash = (int)(fullPath.find_last_of('/'));

	std::string actualPath = fullPath.substr(0, whereDash);

	//char buff[255]; sprintf(buff, "%s/%s", actualPath.c_str(), filename);

	string s = actualPath + string("\\") + string(filename); 

	ifstream fin;

	fin.open(s.c_str());

	if(fin.fail())

	{

		std::cout << "Can't open the file" << std::endl;

		return false;

	}

	string identifierStr;

	bool foundNewMaterial = false;

	ObjMat newMaterial;

	while(!fin.eof())

	{

		fin >> identifierStr;

		if(identifierStr == "#")

		{

			fin.getline(line,255);

			fin >> ws;

		}
		else if(identifierStr == "newmtl")
		{
			//cout << "newmtl" <<endl;
			if(foundNewMaterial)
			{

				//char buf[255];

				//cout << " newmtl " << newMaterial.matName << " " << newMaterial.textureName << endl;

				//sprintf(buf, "%s/%s",actualPath.c_str(),newMaterial.textureName); 
				string s = actualPath + "\\" + newMaterial.textureName;
				newMaterial.glIndex = TextureHandler::lookUpTexture(s, true);

				theMats.push_back(newMaterial);

			}

			fin.getline(line,255); 

			fin >> ws;

			sscanf_s(line, "%s", newMaterial.matName);
			///cout << newMaterial.matName << " MATERIAL NAME>" << endl;

			foundNewMaterial = true;

		}

		else if(identifierStr == "Ns" || identifierStr == "Ni" || identifierStr == "Tr" || identifierStr == "Tf")//skip all these

		{

			fin.getline(line,255);

			fin >> ws;

		}

		else if(identifierStr == "d")

		{

			fin.getline(line, 255);

			fin >> ws;

			sscanf_s(line, " %f", &newMaterial.d);

		}

		else if(identifierStr == "illum")

		{

			fin.getline(line, 255);

			fin >> ws;

			sscanf_s(line, "llum %i", &newMaterial.illum);

		}

		else if(identifierStr == "Ka")

		{

			fin.getline(line, 255); //Ambient

			sscanf_s(line, "%f %f %f", &newMaterial.ambientCol[0], &newMaterial.ambientCol[1], &newMaterial.ambientCol[2]);

		}

		else if(identifierStr == "Kd")

		{

			fin.getline(line, 255); //diffuse

			fin >> ws;

			sscanf_s(line, "Kd %f %f %f", &newMaterial.diffuseCol[0], &newMaterial.diffuseCol[1], &newMaterial.diffuseCol[2]);

		}

		else if(identifierStr == "Ks")

		{

			fin.getline(line, 255); //specular

			fin >> ws;

			sscanf_s(line, "Ks %f %f %f", &newMaterial.specCol[0], &newMaterial.specCol[1], &newMaterial.specCol[2]);

		}

		else if(identifierStr == "Ke")///not used so skip

		{

			fin.getline(line, 255); //emissive

			fin >> ws;

		}

		else if(identifierStr == "map_Kd")

		{

			fin.getline(line, 255); //textureName

			fin >> ws;

			sscanf_s(line, "%s", &newMaterial.textureName); 

		}

		else if(identifierStr == "map_Ka")//skip not used

		{

			fin.getline(line, 255); //textureName

			fin >> ws; 

		}

		else//skip anything else

		{

			fin.getline(line, 255); 

			fin >> ws; 

		}

	}

	if(foundNewMaterial)

	{

		//char buf[255];

		//sprintf(buf, "%s/%s",actualPath.c_str(),newMaterial.textureName); 
		string s = actualPath + "\\" + newMaterial.textureName;
		newMaterial.glIndex = TextureHandler::lookUpTexture(s, true);
		
		cout << "MATERIAL " << newMaterial.textureName << " " << newMaterial.glIndex << endl;

		theMats.push_back(newMaterial);

	}

	std::cout << "Number of Materials Loaded " << (int) theMats.size() << std::endl;

	return true;

}


int OBJLoader::lookupMaterial(char *matName)
{
	for (int count = 0; count < (int) theMats.size(); count++)
	{
		if (strcmp(theMats[count].matName, matName) == 0)
		{
			return count;
		}
	}
	return -1;
}

bool OBJLoader::myFileLoader(char *filename)
{
	char line[255];

	int currentMat = -1;
	
	FILE * theFile;
	fopen_s(&theFile, filename, "rt");
	if(!theFile)
	{
		std::cout << "Can't open the file" << std::endl;
		return false;
	}

	while(!feof(theFile))
	{
		char firstChar = fgetc(theFile);

		switch (firstChar)
		{
		case 'v' :   //It's a vertex or vertex attribut
			{
				char secondChar = fgetc(theFile);
			
				switch (secondChar)
				{
					case ' ':   //space or tab - must be just a vert
					case '\t':
						{
							float thePoints[3];
							fgets(line, 255, theFile); //read in the whole line				
							sscanf_s(line, " %f %f %f", &thePoints[0], &thePoints[1], &thePoints[2]); //get the vertex coords	
							thePoints[0] *= -1.0f;
							thePoints[2] *= -1.0f;
							m_vVertices.push_back(thePoints); //add to the vertex array
							break;
						}
					case 'n':
						{
							float theNormals[3];
							fgets(line, 255, theFile); //get the Normals						
							sscanf_s(line, " %f %f %f", &theNormals[0], &theNormals[1], &theNormals[2]); //get the normal coords							
							m_vNormals.push_back(theNormals); //add to the normal array
							break;
						}
					case 't':
						{
							float theTex[2];
							fgets(line, 255, theFile); //get the Tex						
							sscanf_s(line, " %f %f", &theTex[0], &theTex[1]); //get the texture coords							
							theTex[1] = 1-theTex[1];							
							m_vTexCoords.push_back(theTex); //add to the text-coord array
							break;
						}
				}
				break;
			}
		case 'f' : //Read in a face
			{
				readFaceLine(theFile, currentMat);
				break;
			}
		case 'm' : //It's the materail lib
			{
				char buff[255];
				char buff2[255];
				fgets(line, 255, theFile); //read in the whole line	
				
			//	sscanf(line, "%s ./%s", &buff, &buff2);
				sscanf_s(line, "%s %s", &buff, sizeof(char)*255, &buff2, sizeof(char)*255);
				myMTLLoader(filename, buff2);
				break;
			}
		case 'u' : //Change current Material
			{
				char buff[255];
				char buff2[255];
				fgets(line, 255, theFile); //read in the whole line	
				
				sscanf_s(line, "%s %s", &buff, sizeof(char)*255, &buff2, sizeof(char)*255);
				
				currentMat = lookupMaterial(buff2);

				break;
			}
		default: // A bit we don't know about - skip line
			{
				if ((firstChar != 10))
				{
					fgets(line, 255, theFile); //read and throw away
				}
			}
		}

	}

	m_pVerts = &m_vVertices[0];
	if(m_vTexCoords.size() <= 0)
	{
		m_vTexCoords.push_back(Vector2d(0,0));
		m_pTexCoords = &m_vTexCoords[0];
	}
	else
	{
		m_pTexCoords = &m_vTexCoords[0];
	}
	if (m_vNormals.size() != 0) m_pNormals = &m_vNormals[0];
	else m_pNormals = 0;
	m_pFaces = &m_vFaces[0];


	return true;
}