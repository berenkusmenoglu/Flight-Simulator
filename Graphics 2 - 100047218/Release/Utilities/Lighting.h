
#ifndef _LIGHTING_H
#define _LIGHTING_H

#include <math.h>

template <class T>
class Lighting
{
public:
	static void calcpoint(T a, T b)
	{
		a[0] = a[0] - b[0];
		a[1] = a[1] - b[1];
		a[2] = a[2] - b[2];
	}
	static void calcpointClass(T a, T b, T& A)
	{
		A[0] = a[0] - b[0];
		A[1] = a[1] - b[1];
		A[2] = a[2] - b[2];
	}

	static void normaliseArray(T norm)
	{
		float d = sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);
		if(d==0){
			return;
		}
		norm[0] /=d;
		norm[1] /=d;
		norm[2] /=d;
	}

	static void normaliseClass(T& norm)
	{
		float d = sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);
		if(d==0){
			return;
		}
		norm[0] /=d;
		norm[1] /=d;
		norm[2] /=d;
	}

	static void NormalClass(T a, T b, T c, T& norm)
	{
		T A;
		T B;
		calcpointClass(a, b, A);
		calcpointClass(b, c, B);
		norm[0] = (A[1]*B[2]) - (A[2]*B[1]);
		norm[1] = (A[2]*B[0]) - (A[0]*B[2]);
		norm[2] = (A[0]*B[1]) - (A[1]*B[0]);
		normaliseClass(norm);
		
	}
	static void NormalArray(T a, T b, T c, T norm)
	{
		calcpoint(a, b);
		calcpoint(b, c);
		norm[0] = (a[1]*b[2]) - (a[2]*b[1]);
		norm[1] = (a[2]*b[0]) - (a[0]*b[2]);
		norm[2] = (a[0]*b[1]) - (a[1]*b[0]);
		normaliseArray(norm);
	}
};

#endif _LIGHTING_H