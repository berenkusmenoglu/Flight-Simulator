#include "RandomTerrainGenerator.h"



RandomTerrainGenerator::RandomTerrainGenerator()
{
}


RandomTerrainGenerator::~RandomTerrainGenerator()
{
}

glm::vec3 RandomTerrainGenerator::GetRandomLocations()
{
	//int index =  0 + rand() % 10;

	int x = -20 + rand() % 20;
	int y = 0 + rand() % 200;
	int z = -20 + rand() % 20;

	return glm::vec3(x, 0, z);

}

glm::vec3 RandomTerrainGenerator::GetRandomScales()
{
	//int index =  0 + rand() % 10;

	int x = 1 + rand() % 20;
	int y = 50 + rand() % 200;
	int z = 1 + rand() % 20;

	return glm::vec3(x, 0, z);

}