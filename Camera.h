#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE


#include <glm/glm.hpp>


// Camera contains the properies of the camera the scene is using
// It is responsible for computing the associated GL matrices


class Camera
{

public:
	Camera();
	~Camera();


	//view
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 look;
	glm::vec3 right;

 
	void init(float initDistance, float initAngleX = 0.0f, float initAngleY = 0.0f);

	void resizeCameraViewport(int width, int height);
	void rotateCamera(float xRotation, float yRotation);
 
	glm::mat4 &getProjectionMatrix();
	glm::mat4 &getModelViewMatrix();


	void computeModelViewMatrix();


private:
	float angleX, angleY, distance;   // Camera parameters
	float rangeDistanceCamera[2];
	glm::mat4 projection, modelview, modelview2;  // OpenGL matrices

};


#endif // _CAMERA_INCLUDE

