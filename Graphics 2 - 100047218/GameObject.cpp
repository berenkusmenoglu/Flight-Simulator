#include "GameObject.h"

GameObject::GameObject()
{
	glm::vec3 objectPosition = glm::vec3();
	glm::mat4 objectRotationMatrix = glm::mat4();
	glm::mat4 objectTransformationMatrix = glm::mat4();
	glm::quat transform = glm::quat();

	objectSpeed = 0.0f;
	misileSpeed = 0.0f;
}

void GameObject::MoveObject(float x, float y, float z)
{
	objectPosition += glm::vec3(x, y, z);
	objectTransformationMatrix = glm::translate(glm::mat4(1), objectPosition);
	model.translate(Vector3d(x, y, z));
}

void GameObject::MoveObjectByVector(glm::vec3 newPos)
{
	objectTransformationMatrix = glm::translate(glm::mat4(1), objectPosition);
	objectTransformationMatrix *= glm::mat4_cast(transform);
	model.translate(Vector3d(newPos.x, newPos.y, newPos.z));
}

void GameObject::MoveObjectByQuaternion()
{
	//updates the object positino based on the direction that the object is currently facein
	glm::vec3 move = glm::vec3(0, 0, -1);//get base forwardVector vector

	move = transform*(move);//apply transformation
	glm::normalize(move); //normalize the vector

	objectPosition.x = objectPosition.x + move.x * objectSpeed;  //updeate the position by adding a factor
	objectPosition.y = objectPosition.y + move.y * objectSpeed;	//of the direction components
	objectPosition.z = objectPosition.z + move.z * objectSpeed;

	MoveObjectByVector(objectPosition);

}

void GameObject::applyQuaternion(Direction d)
{
	switch (d)
	{
	case GameObject::LEFT:
	{
		glm::vec3 forwardVec = glm::vec3(0, 0, 1);
		glm::vec3 barrel = glm::vec3(-1, 0, 0);

		double sinAngle;
		glm::vec3 vn = glm::vec3(forwardVec.x, forwardVec.y, forwardVec.z);//gets a vector
		sinAngle = sin(angleDown * rotationSpeed);	//takes sing of have the angle of rotation
		axis1.x = (vn.x * sinAngle);  //sets up the quaternion compnonests
		axis1.y = (vn.y * sinAngle);
		axis1.z = (vn.z * sinAngle);
		axis1.w = cos(angleDown);
		glm::normalize(axis1);
		glm::normalize(transform);

		transform = transform * axis1;

	}
	break;
	case GameObject::RIGHT:
	{
		glm::vec3 forwardVec = glm::vec3(0, 0, 1);
		glm::vec3 barrel = glm::vec3(-1, 0, 0);

		double sinAngle;
		glm::vec3 vn = glm::vec3(forwardVec.x, forwardVec.y, forwardVec.z);//gets a vector
		sinAngle = sin(angleUp * rotationSpeed);	//takes sing of have the angle of rotation
		axis1.x = (vn.x * sinAngle);  //sets up the quaternion compnonests
		axis1.y = (vn.y * sinAngle);
		axis1.z = (vn.z * sinAngle);
		axis1.w = cos(angleUp);
		glm::normalize(axis1);
		glm::normalize(transform);

		transform = transform * axis1;

	}
	break; case GameObject::TURNLEFT:
	{
		glm::vec3 forwardVec = glm::vec3(0, 0, 1);
		glm::vec3 barrel = glm::vec3(0, -1, 0);

		double sinAngle;
		glm::vec3 vn = glm::vec3(barrel.x, barrel.y, barrel.z);//gets a vector
		sinAngle = sin(angleUp * rotationSpeed);	//takes sing of have the angle of rotation
		axis1.x = (vn.x * sinAngle);  //sets up the quaternion compnonests
		axis1.y = (vn.y * sinAngle);
		axis1.z = (vn.z * sinAngle);
		axis1.w = cos(angleUp);
		glm::normalize(axis1);
		glm::normalize(transform);

		transform = transform * axis1;


	}
	break;
	case GameObject::TURNRIGHT:
	{
		glm::vec3 forwardVec = glm::vec3(0, 0, 1);
		glm::vec3 barrel = glm::vec3(0, 1, 0);

		double sinAngle;
		glm::vec3 vn = glm::vec3(barrel.x, barrel.y, barrel.z);//gets a vector
		sinAngle = sin(angleUp * rotationSpeed);	//takes sing of have the angle of rotation
		axis1.x = (vn.x * sinAngle);  //sets up the quaternion compnonests
		axis1.y = (vn.y * sinAngle);
		axis1.z = (vn.z * sinAngle);
		axis1.w = cos(angleUp);
		glm::normalize(axis1);
		glm::normalize(transform);

		transform = transform * axis1;

	}
	break;
	case GameObject::UP:
	{
		glm::vec3 forwardVec = glm::vec3(0, 0, 1);
		glm::vec3 barrel = glm::vec3(-1, 0, 0);

		double sinAngle;
		glm::vec3 vn = glm::vec3(barrel.x, barrel.y, barrel.z);//gets a vector
		sinAngle = sin(angle2down * rotationSpeed);	//takes sing of have the angle of rotation
		axis2.x = (vn.x * sinAngle);  //sets up the quaternion compnonests
		axis2.y = (vn.y * sinAngle);
		axis2.z = (vn.z * sinAngle);
		axis2.w = cos(angle2down);
		glm::normalize(axis2);
		glm::normalize(transform);

		transform = transform * axis2;

	}
	break;
	case GameObject::DOWN:
	{
		glm::vec3 forwardVec = glm::vec3(0, 0, 1);
		glm::vec3 barrel = glm::vec3(-1, 0, 0);

		double sinAngle;
		glm::vec3 vn = glm::vec3(barrel.x, barrel.y, barrel.z);//gets a vector
		sinAngle = sin(angle2up * rotationSpeed);	//takes sing of have the angle of rotation
		axis2.x = (vn.x * sinAngle);  //sets up the quaternion compnonests
		axis2.y = (vn.y * sinAngle);
		axis2.z = (vn.z * sinAngle);
		axis2.w = cos(angle2up);
		glm::normalize(axis2);
		glm::normalize(transform);

		transform = transform * axis2;
	}
	break;
	default:
		break;
	}

	
}

void GameObject::Fire()
{
	misileSpeed = objectSpeed + 5;

}
