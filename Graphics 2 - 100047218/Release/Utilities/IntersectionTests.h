/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-Möller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/

class IntersectionTests
{
public:
	static int objectBoxOverlap(float normal[3],float d, float maxbox[3]);
	static int objectBoxOverlap(double normal[3],double d, double maxbox[3]);
	static int triBoxOverlap(float boxhalfsize[3],float v0[3], float v1[3],float v2[3]);
	static int triBoxOverlap(double boxcenter[3],double boxhalfsize[3],double triverts[3][3]);
	static bool OBB_RAY_Intersect(float fromRay[3], float toRay[3]);
};