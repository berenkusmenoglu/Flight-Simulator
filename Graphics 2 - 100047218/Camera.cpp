#include "Camera.h"

Camera::Camera(glm::vec3 spawnPoint)
{
	glm::vec3 cameraPosition = spawnPoint;
	glm::mat4 cameraRotationMatrix = glm::mat4();
	glm::mat4 cameraTransformMatrix = glm::mat4();
	glm::quat transform = glm::quat();

	cameraforwardVectorFactor = 5.0;
	cameraSpeed = 0.8f;
	

	//////////CAMERAS//////////

	cameras[0] = glm::lookAt(glm::vec3(objectPosition.x, objectPosition.y, objectPosition.z + 10), objectPosition, glm::vec3(0, 1, 0)); // back view dyanmic camera
	cameras[1] = glm::lookAt(glm::vec3(0, 0, 10), objectPosition, glm::vec3(0, 1, 0)); // back view static camera
	cameras[2] = glm::lookAt(glm::vec3(objectPosition.x, objectPosition.y + 10, objectPosition.z + 10), objectPosition, glm::vec3(0, 1, 0)); //top angled view dynamic camera
	cameras[3] = glm::lookAt(glm::vec3(objectPosition.x, objectPosition.y, objectPosition.z - 10), objectPosition, glm::vec3(0, 1, 0)); //front view dynamic camera
	cameras[4] = glm::lookAt(glm::vec3(objectPosition.x, objectPosition.y, objectPosition.z - 5), glm::vec3(objectPosition.x, objectPosition.y, objectPosition.z - 10), glm::vec3(0, 1, 0)); //fps view  dynamic camera



}

void Camera::MoveCamera(float x, float y, float z)
{
	cameraPosition += glm::vec3(x, y, z);
	cameraTransformMatrix = glm::translate(glm::mat4(1), cameraPosition);

}

void Camera::MoveCameraByVector(glm::vec3 newPos)
{
	cameraPosition += newPos;
	cameraTransformMatrix = glm::translate(glm::mat4(1), cameraPosition);

}

void Camera::MoveCameraByQuaternion()
{
	//updates the camera positino based on the direction that the camera is currently facein
	glm::vec3 move = glm::vec3(0, 0, -1);//get base forwardVector vector

	move = transform*(move);//apply transformation
	glm::normalize(move); //normalize the vector

	cameraPosition.x = cameraPosition.x + move.x * cameraSpeed;  //updeate the position by adding a factor
	cameraPosition.y = cameraPosition.y + move.y * cameraSpeed;	//of the direction components
	cameraPosition.z = cameraPosition.z + move.z * cameraSpeed;

	cameraTransformMatrix = glm::translate(glm::mat4(1), cameraPosition);

}

void Camera::UpdateCameras(glm::vec3 airobjectPos, glm::quat objectTransform)
{
	this->objectPosition = airobjectPos;
	this->objectTransform = objectTransform;


	glm::vec3 move = glm::vec3(0, 0, -1);
	move = objectTransform * move;
	glm::normalize(move);

	glm::vec3 dir;
	dir.x = -1 * move.x;
	dir.y = -1 * move.y;
	dir.z = -1 * move.z;

	dir.x *= cameraforwardVectorFactor;					//scale the vector to set forwardVector, backward camera position
	dir.y *= cameraforwardVectorFactor;
	dir.z *= cameraforwardVectorFactor;


	normal = glm::vec3(0, 1, 0);	//get the normal vector of the airobject
	normal = objectTransform * normal;
	glm::normalize(normal);

	double aty = normal.y * cameraHeight;	//set camera hiehgt

	double looky = normal.y * cameraReferenceHeight;

	this->objectPosition = airobjectPos;

	cameras[0] = glm::lookAt(
		glm::vec3(
			objectPosition.x + dir.x,
			objectPosition.y + dir.y + aty,
			objectPosition.z + dir.z),
		glm::vec3(
			objectPosition.x + move.x,
			objectPosition.y + move.y + looky,
			objectPosition.z + move.z),
		glm::vec3(
			normal.x,
			normal.y,
			normal.z)
		); // back view dyanmic camera



	cameras[1] = glm::lookAt(
		glm::vec3(0, 0, 0),
		glm::vec3(
			objectPosition.x + move.x,
			objectPosition.y + move.y + looky,
			objectPosition.z + move.z),
		glm::vec3(0, 1, 0)
		); // back view static camera

	cameras[2] = glm::lookAt(
		glm::vec3(objectPosition.x,
			objectPosition.y + 5,
			objectPosition.z + 10),
		glm::vec3(
			objectPosition.x + move.x,
			objectPosition.y + move.y + looky,
			objectPosition.z + move.z),
		glm::vec3(
			normal.x,
			normal.y,
			normal.z)
		); //top angled view dynamic camera action camera

	cameras[3] = glm::lookAt(
		glm::vec3(
			objectPosition.x - dir.x,
			objectPosition.y - dir.y + aty,
			objectPosition.z - dir.z),
		glm::vec3(
			objectPosition.x + move.x,
			objectPosition.y + move.y + looky,
			objectPosition.z + move.z),
		glm::vec3(
			normal.x,
			normal.y,
			normal.z)
		); //front view dynamic camera

	cameras[4] = glm::lookAt(
		glm::vec3(
			objectPosition.x + dir.x,
			objectPosition.y + dir.y + aty,
			objectPosition.z + dir.z),
		glm::vec3(
			objectPosition.x + move.x,
			objectPosition.y + move.y + looky,
			objectPosition.z + move.z),
		glm::vec3(
			normal.x,
			normal.y,
			normal.z)
		); //fps view  dynamic camera

}

glm::mat4 Camera::UseCamera(int camIndex)
{
	if (camIndex < 5 || camIndex >= 0) {
		UpdateCameras(this->objectPosition, this->objectTransform);
		return cameras[camIndex];
	}
	else
		return cameras[0];

}

