/*******************************************************
				Implementation of Vector3d File
*******************************************************/
#include <math.h>
#include "Vector3d.h"



/*******************************************************
				Vector 3d
*******************************************************/

Vector3d::Vector3d () 
{ 
	 x = 0; y =0; z = 0;
}

Vector3d::Vector3d (float newX, float newY, float newZ)
{
	x = newX; y = newY; z = newZ;
}


Vector3d::Vector3d (float *input)
{	
	x = input[0];
	y = input[1];
	z = input[2];
}

/*******************************************************
				Operators
*******************************************************/

Vector3d Vector3d::operator * (const Vector3d &vector2) //Define Cross Product
{
	Vector3d results((y * vector2.z)-(z * vector2.y),
				(z * vector2.x)-(x * vector2.z),
				(x * vector2.y)-(y * vector2.x));
	return results;
}

Vector3d Vector3d::operator*(float scale)
{
	Vector3d results(x*scale, y*scale, z*scale);
	return results;
}

Vector3d Vector3d::operator/(float scale)
{
	Vector3d results(x/scale, y/scale, z/scale);
	return results;
}


Vector3d Vector3d::operator - (const Vector3d &vector2)
{
	Vector3d results(x - vector2.x, y - vector2.y, z - vector2.z);
	return results;
}


Vector3d Vector3d::operator + (const Vector3d &vector2)
{
	Vector3d results(x + vector2.x, y + vector2.y,  z + vector2.z);
	return results;
}

bool Vector3d::operator != (const Vector3d &vector2)
{
	if ((x != vector2.x) || (y != vector2.y) || (z != vector2.z)) return true;
	else return false;
}


float Vector3d::operator[](unsigned int index)
{
	switch (index)
	{
	case 0: {
				return x;
				break;
			}
		case 1: 
			{
				return y;
				break;
			}
		case 2: 
			{
				return z;
				break;
			}
	}
	
	return 0.0f;
}

Vector3d Vector3d::operator-()
{
	Vector3d results(-x, -y, -z);
	return results;
}

/*******************************************************
				Methods
*******************************************************/


bool Vector3d::isEqual (Vector3d &vector1, Vector3d &vector2, float threshold)
{
	if ((fabs(vector1.x-vector2.x) < threshold) && (fabs(vector1.y-vector2.y) < threshold)  && (fabs(vector1.z-vector2.z) < threshold))
	{
		return true;
	}
	else 
	{
		return false;
	}
}


void Vector3d::normalize()
{
	float factor = sqrt(x*x+y*y+z*z);
	if (factor != 0)
	{
		x /= factor; y/= factor; z /=factor;
	}
}

Vector3d Vector3d::normalize(const Vector3d &toDo)
{
	Vector3d result = toDo;
	float factor = sqrt(result.x*toDo.x+result.y*result.y+result.z*result.z);
	if (factor != 0)
	{
		result.x /= factor; result.y/= factor; result.z /=factor;
	}
	return result;
}

inline float Vector3d::size()
{
	return (float) sqrt((x*x)+(y*y)+(z*z));
}

float Vector3d::sizeSqr()
{
	return (float) (x*x)+(y*y)+(z*z);
}

float Vector3d::angleBetweenVectors(Vector3d &first,Vector3d &second)
{
	float angle;

	float topPart = dotProduct(first,second);
	float bottom = first.size() * second.size();

	angle = (float) acos(topPart/bottom);

	return angle;
}

float Vector3d::signedAngleBetweenVectors(Vector3d &first,Vector3d &second)
{
	float angle;

	float topPart = dotProduct(first,second);
	float bottom = first.size() * second.size();

	angle = (float) acos(topPart/bottom);

	Vector3d cross = first * second;
	float sign = cross.x + cross.y + cross.z;
	int sgn = ( sign > 0 ? 1 : -1);  

	return angle * sgn;
}

float Vector3d::dotProduct(const Vector3d &first, const Vector3d &second)
{
	return (first.x * second.x) + (first.y * second.y) +(first.z * second.z);
}


float Vector3d::angleBetweenVectors2d(const Vector3d &first, const Vector3d &second, char axis)
{	
	Vector3d firstCopy = first;
	Vector3d secondCopy = second;
	
	switch (axis)
	{
	case 'x':
		{
			firstCopy.x = secondCopy.x = 0; break;
		}
	case 'y':
		{
			firstCopy.y = secondCopy.y = 0; break;
		}
	case 'z':
		{
			firstCopy.z = secondCopy.z = 0; break;
		}

	}
	
	float angle;

	float topPart = (firstCopy.x * secondCopy.x) + (firstCopy.y * secondCopy.y) +(firstCopy.z * secondCopy.z);
	float bottom = firstCopy.size() * secondCopy.size();

	angle = (float) acos(topPart/bottom);

	return angle;
}

// Compress vectors ([-1, 1] -> [0, 1])
Vector3d Vector3d::rangeCompress()
{
   Vector3d temp;
   temp.x = (x + 1.0f) / 2.0f;
   temp.y = (y + 1.0f) / 2.0f;
   temp.z = (z + 1.0f)/ 2.0f;
   return temp;
}

void  Vector3d::limitVector(Vector3d &input, float maxSize)
{
	if (input.size() > maxSize)
	{
		input.normalize();
		input = input * maxSize;
	}
}

float Vector3d::xzLength(Vector3d toWorkOut)
{
	return (sqrt(toWorkOut.x * toWorkOut.x + toWorkOut.z * toWorkOut.z));
}

float Vector3d::xzLengthSqr(Vector3d toWorkOut)
{
	return toWorkOut.x * toWorkOut.x + toWorkOut.z * toWorkOut.z;
}