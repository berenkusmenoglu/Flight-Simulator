#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "GameObject.h"

class Camera
{
public:

	
	glm::vec3 cameraPosition;
	glm::vec3 objectPosition;

	glm::mat4 cameraTransformMatrix;
	glm::quat transform;
	glm::quat objectTransform;

	glm::mat4 cameras[10];
	glm::vec3 normal = glm::vec3(0, 1, 0);

	float spinSpeed;
	float cameraSpeed;


	double cameraforwardVectorFactor;
	double cameraHeight = 5;             //default camera height
	double cameraReferenceHeight = 5;


	Camera(glm::vec3 spawnPoint);

	void MoveCamera(float x, float y, float z);
	void MoveCameraByVector(glm::vec3);
	void MoveCameraByQuaternion();

	glm::mat4 UseCamera(int);
	void UpdateCameras(glm::vec3 airobjectPos, glm::quat objectTransform);

};

