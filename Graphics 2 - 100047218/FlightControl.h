#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

#include "Game.h"


class FlightControl
{
public:

	enum GamePlayType
	{
		NORMAL,
		DIFFICULT,
		FUN
	};


	enum PlaneState
	{
		FLYING,
		TAXYING,
		CRASHED
	};

	PlaneState state;

	GamePlayType type;

	FlightControl(GamePlayType type);



	float planeSpeed;
	glm::vec3 planePosition;
	glm::vec3 groundPosition;
	glm::vec3 planeAngles;

	float maxObjectSpeed;
	float minimumTakeOffSpeed;
	float minimumFlyingSpeed;
	float acceptableLandingSpeed;
	float acceptableLandingAngle;

	void updatePlaneState(glm::vec3 planePosition, glm::vec3 groundPosition, float planeSpeed, glm::vec3 angles);

	bool allowRotation(Game thisGame);
	bool allowTakeOff(Game thisGame);
	bool validLanding();

};

