#pragma once
#include "GameObject.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

class Airplane : GameObject

{
public:

	Airplane();
	GameObject planeObject;
	GameObject getPlane();

};

