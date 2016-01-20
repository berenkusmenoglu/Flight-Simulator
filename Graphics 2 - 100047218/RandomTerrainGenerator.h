#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include <cstdlib>
/*
HOLDS AN ARRAY OF MODEL POSITIONS TO RANDOMLY GENERATE A TERRAIN
Model with different positions are kept in the array and randomly `CALLED` by the main method;
*/
class RandomTerrainGenerator
{
public:
	RandomTerrainGenerator();
	~RandomTerrainGenerator();

	glm::vec3 GetRandomLocations();
	glm::vec3 GetRandomScales();
};

