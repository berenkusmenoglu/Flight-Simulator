#ifndef _VECTOR_2D
#define _VECTOR_2D

#include <iostream>
#include <fstream>

class Vector2d
{
public:
	float x,y;

	Vector2d();
	Vector2d (float newX, float newY);
	Vector2d (float *input);

	float size();
	void normalize();			//Normalise a Vector

	Vector2d operator*(float scale);  //size multiply

	Vector2d operator+(const Vector2d &);	//Vector addition
	Vector2d operator-(const Vector2d &);	//Vector subtraction

	friend std::ostream& operator << (std::ostream &os,const Vector2d &obj)
	{
		os << "X: "<< obj.x << " Y: " << obj.y;
		return os;
	}	
	static float dotProduct(const Vector2d &first, const Vector2d &second);	

};

#endif