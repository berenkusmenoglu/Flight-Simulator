
#ifndef _OCTREE_H
#define _OCTREE_H

#include <vector>
using namespace std;

#include "../3DStruct/threeDModel.h"
#include "../Box.h"

class Shader;

#define MAX_DEPTH  1

class Octree
{
private:
	double minX, minY, minZ, maxX, maxY, maxZ;
	Octree* children[8];
	int Level;	
	int* PrimitiveList;
	int PrimitiveListSize;

	int* VertexList;
	int VertexListSize;

	Box* box;

public:
	Octree();
	~Octree();
	void getMinValues(double* min);
	void getMaxValues(double* max);
	int* getPrimitiveList();
	int getPrimitiveListSize();
	void Delete();
	void set(int L, float x, float y, float z, float X, float Y, float Z, int* PrimList, int PrimListSize, int* vertList, int VertListSize);
	void start(int L, float x, float y, float z, float X, float Y, float Z, ThreeDModel* model);
	void CreateChildren(vector<Octree*> &stackOctree, ThreeDModel* model);
	void CalculatePrimitiveWithinSubdividedOctreeCells(ThreeDModel* model, int primitiveIndex, int** primitiveSets, int primitiveSetsCount[8]);
	void CalculateVerticesWithinSubdividedOctreeCells(ThreeDModel* model, int vertexIndex, int** vertexSets, int vertexSetsCount[8]);
	void setUpAccessor(int** Acc, float GlobalMinX, float boxWidthX, float GlobalMinY, float boxWidthY, float GlobalMinZ, float boxWidthZ);
	void drawAllBoxes(Shader* myShader);
	void drawBoxesAtLeaves(Shader* myShader);
	void drawBoundingBox(Shader* myShader);
	Octree* getChild(int i);
	int getLevel();
	void processVerticesByLeaf(ThreeDModel* model);
};

#endif _OCTREE_H