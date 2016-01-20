#ifndef _MATRIX_ROUTINES_H
#define _MATRIX_ROUTINES_H

#include <math.h>

#define PI 3.1415926535897932384626433832795
const double ToRadians = PI / 180.0;

template <class T>
class MatrixRoutines
{
public:
	static void setToIdentity(T m[16])
	{
		for(int i =0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				if(i==j)
					m[i*4+j] = 1.0;
				else
					m[i*4+j] = 0.0;
			}
		}
	}
	static void setTranslation(T x, T y, T z, T m[16])
	{
		setToIdentity(m);

		m[12] = x;
		m[13] = y;
		m[14] = z;
	}
	static void setRotationX(T spinInDegrees, T m[16])
	{
		setToIdentity(m);

		m[5] = cos(spinInDegrees*ToRadians); 
		m[6] = -sin(spinInDegrees*ToRadians);
		m[9] = sin(spinInDegrees*ToRadians);
		m[10]= cos(spinInDegrees*ToRadians);
	}
	static void setRotationY(T spinInDegrees, T m[16])
	{
		setToIdentity(m);

		m[0] = cos(spinInDegrees*ToRadians); 
		m[2] = sin(spinInDegrees*ToRadians);
		m[8] = -sin(spinInDegrees*ToRadians);
		m[10]= cos(spinInDegrees*ToRadians);
	}
	static void setRotationZ(T spinInDegrees, T m[16])
	{
		setToIdentity(m);

		m[0] = cos(spinInDegrees*ToRadians); 
		m[1] = -sin(spinInDegrees*ToRadians);
		m[4] = sin(spinInDegrees*ToRadians);
		m[5]= cos(spinInDegrees*ToRadians);
	}

	static void matrixMultiply4x4RigidBody(T A[16], T B[16], T C[16])
	{
		float D[16];
		memcpy_s(D, sizeof(T)*16, A, sizeof(T)*16);
	
		C[0] = D[0]*B[0] + D[4]*B[1] + D[8]*B[2];
		C[1] = D[1]*B[0] + D[5]*B[1] + D[9]*B[2];
		C[2] = D[2]*B[0] + D[6]*B[1] + D[10]*B[2];
		C[3] = 0;

		C[4] = D[0]*B[4] + D[4]*B[5] + D[8]*B[6];
		C[5] = D[1]*B[4] + D[5]*B[5] + D[9]*B[6];
		C[6] = D[2]*B[4] + D[6]*B[5] + D[10]*B[6];
		C[7] = 0;

		C[8] = D[0]*B[8] + D[4]*B[9] + D[8]*B[10];
		C[9] = D[1]*B[8] + D[5]*B[9] + D[9]*B[10];
		C[10] = D[2]*B[8] + D[6]*B[9] + D[10]*B[10];
		C[11] = 0;

		C[12] = D[0]*B[12] + D[4]*B[13] + D[8]*B[14] + D[12];
		C[13] = D[1]*B[12] + D[5]*B[13] + D[9]*B[14] + D[13];
		C[14] = D[2]*B[12] + D[6]*B[13] + D[10]*B[14] + D[14];
		C[15] = 1;
	}
	static void NormalMatrix(T mat[16], T normalMat[9])
	{
		//The normal matrix is the inverse transpose of the modelview matrix

		T d = (mat[0] * (mat[5] * mat[10] - mat[6] * mat[9]))
                  - (mat[1] * (mat[4] * mat[10] - mat[6] * mat[8]))
                  + (mat[2] * (mat[4] * mat[9] - mat[5] * mat[8]));

		if(fabs(d) < 0.001)
		{
			setToIdentity(normalMat);
		}
		else
		{
			 d = 1.0f / d;

			normalMat[0] = d * (mat[5] * mat[10] - mat[6] * mat[9]);
			normalMat[3] = d * (mat[2] * mat[9] - mat[1] * mat[10]);
			normalMat[6] = d * (mat[1] * mat[6] - mat[2] * mat[5]);

			normalMat[1] = d * (mat[6] * mat[8] - mat[4] * mat[10]);
			normalMat[4] = d * (mat[0] * mat[10] - mat[2] * mat[8]);
			normalMat[7] = d * (mat[2] * mat[4] - mat[0] * mat[6]);

			normalMat[2] = d * (mat[4] * mat[9] - mat[5] * mat[8]);
			normalMat[5] = d * (mat[1] * mat[8] - mat[0] * mat[9]);
			normalMat[8] = d * (mat[0] * mat[5] - mat[1] * mat[4]);
		}
	}
	static void multiplyPoint(T point[3], T matrix[16], T resultingPoint[3])
	{
		resultingPoint[0] = matrix[0]*point[0] + matrix[4]*point[1] + matrix[8]*point[2] + matrix[12];
		resultingPoint[1] = matrix[1]*point[0] + matrix[5]*point[1] + matrix[9]*point[2] + matrix[13];
		resultingPoint[2] = matrix[2]*point[0] + matrix[6]*point[1] + matrix[10]*point[2] + matrix[14];
	}

	static void MultiplyRotationVector(T point[3], T matrix[16], T resultingPoint[3])
	{
		resultingPoint[0] = matrix[0]*point[0] + matrix[4]*point[1] + matrix[8]*point[2];
		resultingPoint[1] = matrix[1]*point[0] + matrix[5]*point[1] + matrix[9]*point[2];
		resultingPoint[2] = matrix[2]*point[0] + matrix[6]*point[1] + matrix[10]*point[2];
	}
	static void Ortho2D(T left, T right, T bottom, T top, T m[16])
	{
		float zNear=-1.0; float zFar=1.0;

		float sx = 2.0f / (right - left);
		float sy = 2.0f / (top - bottom);
		float sz = 2.0f / (zFar - zNear);

		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(zFar + zNear) / (zFar - zNear);

		//matrix is stored column major
		m[0] = sx,	 m[4] = 0.0f, m[ 8] = 0.0f,  m[12] = tx;
		m[1] = 0.0f, m[5] = sy,	  m[ 9] = 0.0f,  m[13] = ty;
		m[2] = 0.0f, m[6] = 0.0f, m[10] = sz,	 m[14] = tz;
		m[3] = 0.0f, m[7] = 0.0f, m[11] = 0.0f,	 m[15] = 1.0f;
	}
	static void perspective(T fovx, T aspect, T znear, T zfar, T m[16])
	{
		float e = 1.0f / tanf((ToRadians*fovx) / 2.0f);
		float aspectInv = 1.0f / aspect;
		float fovy = 2.0f * atanf(aspectInv / e);
		float xScale = 1.0f / tanf(0.5f * fovy);
		float yScale = xScale / aspectInv;

		m[0] = xScale;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;

		m[4] = 0.0f;
		m[5] = yScale;
		m[6] = 0.0f;
		m[7] = 0.0f;

		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = (zfar + znear) / (znear - zfar);
		m[11] = -1.0f;

		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = (2.0f * zfar * znear) / (znear - zfar);
		m[15] = 0.0f;
	}
};

#endif _MATRIX_ROUTINES_H