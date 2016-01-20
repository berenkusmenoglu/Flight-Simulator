#include <math.h>
#include "Vector2d.h"


/*******************************************************
				Vector 2d
*******************************************************/

Vector2d::Vector2d()
{
	x = 0;
	y = 0;
}

Vector2d::Vector2d (float newX, float newY)
{
	x = newX;
	y = newY;
}
Vector2d::Vector2d (float *input)
{
	x = input[0];
	y = input[1];
}

Vector2d Vector2d::operator*(float scale)
{
	Vector2d results(x*scale, y*scale);
	return results;
}

float Vector2d::dotProduct(const Vector2d &first, const Vector2d &second)
{
	return (first.x * second.x) + (first.y * second.y);
}

float Vector2d::size()
{
	return (float) sqrt((x*x)+(y*y));
}

void Vector2d::normalize()
{
	float factor = sqrt(x*x+y*y);
	x /= factor; y/= factor;;
}
Vector2d Vector2d::operator - (const Vector2d &vector2)
{
	Vector2d results(x - vector2.x, y - vector2.y);
	return results;
}


Vector2d Vector2d::operator + (const Vector2d &vector2)
{
	Vector2d results(x + vector2.x, y + vector2.y);
	return results;
}