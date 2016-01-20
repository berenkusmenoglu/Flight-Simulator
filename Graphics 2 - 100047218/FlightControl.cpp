#include "FlightControl.h"



FlightControl::FlightControl(GamePlayType type)
{
	switch (type)
	{
	case FlightControl::NORMAL:
		minimumTakeOffSpeed = 0.7;

		minimumFlyingSpeed = 0.7;
		acceptableLandingSpeed = 0.8;
		acceptableLandingAngle = .20;
		maxObjectSpeed = 1;


		break;
	case FlightControl::DIFFICULT:
		minimumTakeOffSpeed = 0.7;

		minimumFlyingSpeed = 0.7;
		acceptableLandingSpeed = 0.8;
		acceptableLandingAngle = .15;
		maxObjectSpeed = 1;

		break;
	case FlightControl::FUN:
		minimumTakeOffSpeed = 0.3;

		minimumFlyingSpeed = 0.3;
		acceptableLandingSpeed = 0.4;
		acceptableLandingAngle = .50;
		maxObjectSpeed = 2;

		break;
	default:
		break;
	}



	state = TAXYING;
}

void FlightControl::updatePlaneState(glm::vec3 givenPlanePos, glm::vec3 givenGroundPos, float givenSpeed, glm::vec3 angles)
{
	planePosition = givenPlanePos;
	groundPosition = givenGroundPos;
	planeAngles = angles;

	float tmpspeed = glm::abs(givenSpeed);
	planeSpeed = tmpspeed;

	if (planePosition.y > groundPosition.y + 10)
	{
		if (planeSpeed >= minimumFlyingSpeed)
			state = FLYING;

	}
	else if ((planePosition.y <= groundPosition.y + 5))
	{
		if ((planeSpeed < minimumTakeOffSpeed))
			state = TAXYING;

	}


}

bool FlightControl::allowRotation(Game thisGame)
{
	if (thisGame.PLAYING)
	{
		return ((state == FLYING));
	}
}

bool FlightControl::allowTakeOff(Game thisGame)
{
	if (thisGame.PLAYING && state == TAXYING)
	{
		return (planeSpeed >= minimumTakeOffSpeed);
	}

}


bool FlightControl::validLanding()
{
	if ((-acceptableLandingAngle <= planeAngles.x) && (planeAngles.x <= acceptableLandingAngle))
	{
		if ((-acceptableLandingAngle <= planeAngles.y) && (planeAngles.y <= acceptableLandingAngle))
		{
			if ((-acceptableLandingAngle <= planeAngles.z) && (planeAngles.z <= acceptableLandingAngle))
				return true;
		}
	}


	return false;

}




