#include "Camera.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


#define PI 3.14159f


Camera::Camera() :
	position(0, 0, 1),
	look(0, 0, -1),
	right(1, 0, 0),
	up(0, 1, 0)
{
}

Camera::~Camera()
{
}


void Camera::init(float initDistance, float initAngleX, float initAngleY)
{
	distance = initDistance;
	angleX = initAngleX;
	angleY = initAngleY;
	rangeDistanceCamera[0] = 1.0f;
	rangeDistanceCamera[1] = 3.0f;
	computeModelViewMatrix();
}

void Camera::resizeCameraViewport(int width, int height)
{
	projection = glm::perspective(60.f / 180.f * PI, float(width) / float(height), 0.05f, 300.0f);
}

 
void Camera::rotateCamera(float xRotation, float yRotation)
{
	look = glm::mat3(glm::rotate(-xRotation, up))*look;
	right = glm::mat3(glm::rotate(-xRotation, up))*right;

	look = glm::mat3(glm::rotate(-yRotation, right))*look;

	computeModelViewMatrix();
}

 


void Camera::computeModelViewMatrix()
{
	modelview = glm::lookAt(position, position + look, up);
}

glm::mat4 &Camera::getProjectionMatrix()
{
	return projection;
}

glm::mat4 &Camera::getModelViewMatrix()
{
	return modelview;
}


