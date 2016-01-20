#include "BoundingBox.h"	

BoundingBox::BoundingBox()
{
	centrePoint = Vector3d(0,0,0);
	boxWidthX = 0;
	boxWidthY = 0;
	boxWidthZ = 0;
}

BoundingBox::BoundingBox(Vector3d newCen, float newWidthX, float newWidthY, float newWidthZ)
{
	centrePoint = newCen;
	boxWidthX = newWidthX;
	boxWidthY = newWidthY;
	boxWidthZ = newWidthZ;
}

float BoundingBox::getLargestExtent()
{
	if ((boxWidthX > boxWidthY) && (boxWidthX > boxWidthZ)) return boxWidthX;
	else if ((boxWidthY > boxWidthX) && (boxWidthY > boxWidthZ)) return boxWidthY;
	else return boxWidthZ;
}


BoundingBox BoundingBox::combineTwoBBoxes(BoundingBox &box1, BoundingBox &box2)
{
	BoundingBox result;

	float minX = min(box1.left(), box2.left());
	float minY = min(box1.bottom(), box2.bottom());
	float minZ = min(box1.front(), box2.front());

	float maxX = max(box1.right(), box2.right());
	float maxY = max(box1.top(), box2.top());
	float maxZ = max(box1.back(), box2.back());

	float	xWidth = (maxX-minX);
	float	yWidth = (maxY-minY);
	float	zWidth = (maxZ-minZ);

	result.centrePoint = Vector3d(minX + (xWidth / 2.0f),minY + (yWidth / 2.0f), minZ + (zWidth / 2.0f));

	result.boxWidthX = xWidth;
	result.boxWidthY = yWidth;
	result.boxWidthZ = zWidth;

	return result;
}

void BoundingBox::makeSquareXZ()
{
	if (boxWidthX < boxWidthZ) boxWidthX = boxWidthZ;
	else boxWidthZ = boxWidthX;
}


float BoundingBox::left() const 
{		//x min
	return float(centrePoint.x) - (boxWidthX/2.0f);
}

float BoundingBox::right() const
{		//x max
	return float(centrePoint.x) + (boxWidthX/2.0f);
}

float BoundingBox::bottom() const
{		//y min
	return float(centrePoint.y) - (boxWidthY/2.0f);
}

float BoundingBox::top() const
{		//y max
	return float(centrePoint.y) + (boxWidthY/2.0f);
}

float BoundingBox::front() const
{		//z min
	return float(centrePoint.z) - (boxWidthZ/2.0f);
}

float BoundingBox::back() const
{		//z max
	return float(centrePoint.z) + (boxWidthZ/2.0f);
}
