#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "3dStruct\threeDModel.h"

class GameObject:ThreeDModel
{
public:

	float rotationRate = 0.003;
	double angleUp = rotationRate * -1;;   //angles for the rotations
	double angleDown = rotationRate;
	double angle2up = rotationRate * -1;
	double angle2down = rotationRate;


	glm::quat axis1, axis2;
	glm::vec3 objectPosition;
	glm::mat4 objectTransformationMatrix;
	glm::quat transform;


	ThreeDModel model;

	enum Direction
	{
		LEFT,
		RIGHT,
		TURNLEFT,
		TURNRIGHT,
		UP,
		DOWN
	};

	float objectSpeed;
	float rotationSpeed = 3;
	float misileSpeed = 0;

	GameObject();

	void MoveObject(float x, float y, float z);
	void MoveObjectByVector(glm::vec3);
	void MoveObjectByQuaternion();

	void applyQuaternion(Direction d);
	void Fire();
};

