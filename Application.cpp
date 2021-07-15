#include <GL/glew.h>
#include <GL/glut.h>
#include "Application.h"
#include <iostream>


void Application::init()
{
	bPlay = true;
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	scene.init();
	
	for(unsigned int i=0; i<256; i++)
	{
	  keys[i] = false;
	  specialKeys[i] = false;
	}
	mouseButtons[0] = false;
	mouseButtons[1] = false;
	
}

bool Application::loadMesh(const char *filename)
{
  return scene.loadMesh(filename);
}

bool Application::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Application::resize(int width, int height)
{
  glViewport(0, 0, width, height);
  scene.getCamera().resizeCameraViewport(width, height);
}

void Application::keyPressed(int key)
{
	if (key == 27) // Escape code
		bPlay = false;
	keys[key] = true;

	if (key == 108)//L
	{
		cout << "\r" << "intentsity : " << scene.intentsity;
	
		// scene.getCamera().position += 0.516f *scene.getCamera().look;
		scene.intentsity += 1;

	}
	if (key == 107)//K
	{
		cout << "\r" << "intentsity : " << scene.intentsity;

		// scene.getCamera().position += 0.516f *scene.getCamera().look;
		scene.intentsity -= 1;

	}

	if (key == 111)//o
	{
		cout << "\r" << "radius : " << scene.radius;
	
		// scene.getCamera().position += 0.516f *scene.getCamera().look;
		scene.radius += .025;

	}
	if (key == 112)//p
	{/////////////////////////////////Changing the values from keyboard
		cout << "\r" << "radius : " << scene.radius;
	
		scene.radius -= .025;

	}
	if (key == 109)//M
	{
		cout << "\r" << "kernelSize : " << scene.kernelSize;

		scene.kernelSize += 10;
		if (scene.kernelSize >= 128) {
			scene.kernelSize = 128;
		}
	}

	if (key == 110)//N
	{
		cout << "\r" << "kernelSize : " << scene.kernelSize;

		// scene.getCamera().position += 0.516f *scene.getCamera().look;
		scene.kernelSize -= 5;
		if (scene.kernelSize <= 0) {
			scene.kernelSize =0;
		}

	}

	/////////////////////////// navigating in the scene WSAD
	if (key == 119)//w
	{
	 
	
		 scene.getCamera().position += 0.516f *scene.getCamera().look;
		//scene.radius += .025;

	}
	if (key == 115)//s
	{
		scene.getCamera().position += -0.516f * scene.getCamera().look;
	}
	if (key == 97)//a
	{

		scene.getCamera().position += -0.516f * scene.getCamera().right;
	}
	if (key == 100)//d
	{

		scene.getCamera().position += 0.516f * scene.getCamera().right;
	}
	if (key == 101)//d
	{

		scene.getCamera().position += 0.5f * scene.getCamera().up;
	}
	if (key == 113)//q
	{

		scene.getCamera().position -= 0.5f * scene.getCamera().up;
	}


	scene.getCamera().computeModelViewMatrix();
}


void Application::keyReleased(int key)
{
	keys[key] = false;
}

void Application::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Application::specialKeyReleased(int key)
{
	specialKeys[key] = false;
	if(key == GLUT_KEY_F1)
	  scene.switchPolygonMode();
	if (key == GLUT_KEY_F2)
		scene.switchOQMode();

	
}

void Application::mouseMove(int x, int y)
{

	// Rotation
	scene.getCamera().rotateCamera((x - lastMousePos.x) / 155, (y - lastMousePos.y) / 155);
	lastMousePos = glm::vec2(x, y);
	//cout << x - lastMousePos.y << endl;

 }
 
void Application::mousePress(int button)
{
 // mouseButtons[button] = true;
}

void Application::mouseRelease(int button)
{
 // mouseButtons[button] = false;
  //if(!mouseButtons[0] && !mouseButtons[1])
    //lastMousePos = glm::ivec2(-1, -1);
}

bool Application::getKey(int key) const
{
	return keys[key];
}

bool Application::getSpecialKey(int key) const
{
	return specialKeys[key];
}





