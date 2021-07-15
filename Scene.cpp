#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Scene.h"
#include "PLYReader.h"
#include "GL/glew.h"
#include  "Mesh.h"
#include <random>
Scene::Scene()
{
	mesh = NULL;
}

Scene::~Scene()
{
	if(mesh != NULL)
		delete mesh;
}


void Scene::init()
{
	initShaders();
	mesh = new TriangleMesh();
	mesh->buildCube();
	mesh->sendToOpenGL(basicProgram);
	currentTime = 0.0f;
	
	camera.init(1.0f);
	
	bPolygonFill = true;
	 n = 0;
			
			
	
	/*locs[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0 * 2, 0, 0 * 2));
	locs[1] = glm::translate(glm::mat4(1.0f), glm::vec3(2 * 2, 0, 0 * 2));
	locs[2] = glm::translate(glm::mat4(1.0f), glm::vec3(0 * 2, 0, 2 * 2));
	locs[3] = glm::translate(glm::mat4(1.0f), glm::vec3(2 * 2, 0, 2 * 2));*/
  	
     object=   new  Mesh("models/teapot.obj", basicProgram, locs);
	 object2 = new  Mesh("models/teapot.obj", basicProgram, locs);
	 floor = new  Mesh("models/floor2.obj", basicProgram, locs);
	 
 
	// tile = new Mesh("models/humen.obj", basicProgram,locs[5]);
	 //wall = new Mesh("models/humen.obj", basicProgram, f2f);

	 //wall = new Mesh("models/wall.obj", basicProgram);
	// Select which font you want to use
	if(!text.init("fonts/OpenSans-Regular.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	 
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	
	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 640, 480, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 640, 480, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments); 




	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 640, 480);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// SSAO_SX color buffer

	glGenFramebuffers(1, &ssao_SX_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_SX_FBO);

	glGenTextures(1, &ssao_sx_ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssao_sx_ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 480, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_sx_ColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO-SX Framebuffer not complete!" << std::endl;

	glGenFramebuffers(1, &ssao_SY_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_SY_FBO);

	glGenTextures(1, &ssao_sy_ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssao_sy_ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 480, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_sy_ColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO-SX Framebuffer not complete!" << std::endl;

	glGenFramebuffers(1, &ssao_SFinal_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssao_SFinal_FBO);

	glGenTextures(1, &ssao_sFinal_ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssao_sFinal_ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 480, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_sFinal_ColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO-SX Framebuffer not complete!" << std::endl;




	glGenFramebuffers(1, &ssaoFBO); 
	glGenFramebuffers(1, &ssaoBlurFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	
	// SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 480, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;
	
	

	//bluring
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 480, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0

	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	 glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (unsigned int i = 0; i < 128; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 128.0;
		

		// scale samples s.t. they're more aligned to center of kernel
		scale = 0.1f+  (scale * scale)*(1-.1f);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}





}

void Scene::renderQuad(GLint tex)
{
	QuadSahder.use();

	QuadSahder.setUniform1i("tex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

 

	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void Scene::renderQuad()
{
	 


	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
bool Scene::loadMesh(const char *filename)
{
	PLYReader reader;
	
	mesh->free();
	bool bSuccess = reader.readMesh(filename, *mesh);
	if(bSuccess)
	  mesh->sendToOpenGL(basicProgram);
	
	return bSuccess;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}
void Scene::BuildScene() {
	  

	 
}

void Scene::render()
{

///////////////////////////////
	

	locs = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 1));
	locs2 = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0,5));
	locs3 = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, 0));

	
	glm::mat3 normalMatrix;
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Geometry.use();
	Geometry.setUniformMatrix4f("projection", glm::translate(camera.getProjectionMatrix(), glm::vec3(0)));
	Geometry.setUniformMatrix4f("modelview", glm::translate(camera.getModelViewMatrix(), glm::vec3(0)));
	normalMatrix = glm::inverseTranspose(camera.getModelViewMatrix());
	Geometry.setUniformMatrix3f("normalMatrix", normalMatrix);
	Geometry.setUniformMatrix4f("model_mat", locs);
	
	
	
	 
	
	object->render();
	Geometry.setUniformMatrix4f("model_mat", locs2);
	object2->render();
	Geometry.setUniformMatrix4f("model_mat", locs3);
	floor->render();

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	///////////////////////////////////////////////////////Toggling between the normal and seprable
	if (!noramlSSAO) {/////////////if we pick the new sperable methiod we will have 5 stages (5 shaders)
		////////// 1 ssaox  2 ssaoy 3 ssaofinal  4 bluring   5 lighting
		glBindFramebuffer(GL_FRAMEBUFFER, ssao_SX_FBO);
		glClear(GL_COLOR_BUFFER_BIT);
		SSAO_SX.use();
		//Send kernel + rotation 
		for (unsigned int i = 0; i < 128; ++i)
			SSAO_SX.setUniform3f("samples[" + std::to_string(i) + "]", ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);
		SSAO_SX.setUniformMatrix4f("projection", glm::translate(camera.getProjectionMatrix(), glm::vec3(0)));
		SSAO_SX.setUniform1f("radius", radius);
		SSAO_SX.setUniform1i("intentsity", intentsity);
		SSAO_SX.setUniform1i("kernelSize", kernelSize);


		SSAO_SX.setUniform1i("gPosition", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);

		SSAO_SX.setUniform1i("gNormal", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);


		SSAO_SX.setUniform1i("texNoise", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);



		renderQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, ssao_SY_FBO);
		glClear(GL_COLOR_BUFFER_BIT);
		SSAO_SY.use();
		//Send kernel + rotation 
		for (unsigned int i = 0; i < 128; ++i)
			SSAO_SY.setUniform3f("samples[" + std::to_string(i) + "]", ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);
		SSAO_SY.setUniformMatrix4f("projection", glm::translate(camera.getProjectionMatrix(), glm::vec3(0)));
		SSAO_SY.setUniform1f("radius", radius);
		SSAO_SY.setUniform1i("intentsity", intentsity);
		SSAO_SY.setUniform1i("kernelSize", kernelSize);


		SSAO_SY.setUniform1i("gPosition", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);

		SSAO_SY.setUniform1i("gNormal", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);


		SSAO_SY.setUniform1i("texNoise", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);



		renderQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);




		glBindFramebuffer(GL_FRAMEBUFFER, ssao_SFinal_FBO);
		glClear(GL_COLOR_BUFFER_BIT);
		SSAO_SFinal.use();
		SSAO_SFinal.setUniform1i("ssaoSX", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssao_sx_ColorBuffer);
		SSAO_SFinal.setUniform1i("ssaoSY", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ssao_sy_ColorBuffer);

		renderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//renderQuad(ssao_sFinal_ColorBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		BlurShader.use();
		BlurShader.setUniform1i("ssaoInput", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssao_sFinal_ColorBuffer);


		renderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//renderQuad(ssaoColorBufferBlur);



	}
	if (noramlSSAO) {////////////// if we chose the noraml ssao we will have 3 stages only (3 shaders ) 1-ssao ,2bluring stage, 3 lighting stage

		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);/////////1 ssao (1 step )
		glClear(GL_COLOR_BUFFER_BIT);
		SSAO.use();
		//Send kernel + rotation 
		for (unsigned int i = 0; i < 128; ++i)
		SSAO.setUniform3f("samples[" + std::to_string(i) + "]", ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);
		SSAO.setUniformMatrix4f("projection", glm::translate(camera.getProjectionMatrix(), glm::vec3(0)));
		SSAO.setUniform1f("radius", radius);
		SSAO.setUniform1i("intentsity", intentsity);
		SSAO.setUniform1i("kernelSize", kernelSize);


		SSAO.setUniform1i("gPosition", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);

		SSAO.setUniform1i("gNormal", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);


		SSAO.setUniform1i("texNoise", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);



		renderQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		
		///////////////////////////////////////////////////////// After we calculated ssao we do  bluring part 
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		BlurShader.use();
		BlurShader.setUniform1i("ssaoInput", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		
		
		renderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//renderQuad(ssaoColorBufferBlur);

	}
		/////////////////////////////////// here both ways will meet in this point  to calculate the final lighitng pass
		// 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LightShader.use();
		// send light relevant uniforms
		glm::vec3 lightPosView = glm::vec3(camera.getProjectionMatrix() * glm::vec4(lightPos, 1.0));
		LightShader.setUniform3f("light.Position", lightPosView.x,lightPosView.y,lightPosView.z);
		LightShader.setUniform3f("light.Color", lightColor.x, lightColor.y, lightColor.z);
		// Update attenuation parameters
		const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const float linear = 0.09;
		const float quadratic = 0.032;
		LightShader.setUniform1f("light.Linear", linear);
		LightShader.setUniform1f("light.Quadratic", quadratic);
		
		LightShader.setUniform1i("gPosition", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		
		LightShader.setUniform1i("gNormal", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		
		LightShader.setUniform1i("gAlbedo" , 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedo);
		
		LightShader.setUniform1i("ssao", 3);
		glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		renderQuad();
	 

		basicProgram.setUniform1i("bLighting", bPolygonFill ? 1 : 0);

		if (bPolygonFill)
		{
			basicProgram.setUniform4f("color", 0.9f, 0.9f, 0.95f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			basicProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(0.5f, 1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			mesh->render();
			glTranslatef(4.0f, 0, 0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_POLYGON_OFFSET_FILL);
			basicProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);

		






	}

	 	text.render("Mesh renderer", glm::vec2(20, 20), 16, glm::vec4(0, 0, 0, 1));
}

Camera &Scene::getCamera()
{
  return camera;
}

void Scene::switchPolygonMode()
{
  bPolygonFill = !bPolygonFill;
}


void Scene::switchOQMode() {
	noramlSSAO = !noramlSSAO;
}
 
void Scene::initShaders()
{/////////////////////////////// he defualt  shader (we dont use it here)
	Shader vShader, fShader;
	Shader G_vShader, G_fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/basic.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/basic.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	basicProgram.init();
	basicProgram.addShader(vShader);
	basicProgram.addShader(fShader);
	basicProgram.link();
	if (!basicProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << basicProgram.log() << endl << endl;
	}
	basicProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
	///////////////////////////////////////////////// G buffer (postion , nomral , albedo)



	vShader.initFromFile(VERTEX_SHADER, "shaders/G-buffer.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/G-buffer.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	Geometry.init();
	Geometry.addShader(vShader);
	Geometry.addShader(fShader);
	Geometry.link();
	if (!Geometry.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << Geometry.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
	////////////////////////////////////vShader.initFromFile(VERTEX_SHADER, "shaders/G-buffer.vert");
	
	
	
	vShader.initFromFile(VERTEX_SHADER, "shaders/SSAO.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/SSAO.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}

	SSAO.init();
	SSAO.addShader(vShader);
	SSAO.addShader(fShader);
	SSAO.link();
	if (!SSAO.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << SSAO.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
//////////////////////////////////////////////////////////////////// to just render A quad on the screen (no texture)
	vShader.initFromFile(VERTEX_SHADER, "shaders/Quad.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/Quad.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	QuadSahder.init();
	QuadSahder.addShader(vShader);
	QuadSahder.addShader(fShader);
	QuadSahder.link();
	if (!QuadSahder.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << QuadSahder.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
////////////////////////////////////////////////////
	vShader.initFromFile(VERTEX_SHADER, "shaders/light.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/light.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	LightShader.init();
	LightShader.addShader(vShader);
	LightShader.addShader(fShader);
	LightShader.link();
	if (!LightShader.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << LightShader.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
	///////////////////////////////////////////////
	vShader.initFromFile(VERTEX_SHADER, "shaders/BlurShader.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/BlurShader.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	BlurShader.init();
	BlurShader.addShader(vShader);
	BlurShader.addShader(fShader);
	BlurShader.link();
	if (!BlurShader.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << BlurShader.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
	////////////////////////////////////////////////////////////////// three shader in order to do the new method
	vShader.initFromFile(VERTEX_SHADER, "shaders/SSAO-SX.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/SSAO-SX.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	SSAO_SX.init();
	SSAO_SX.addShader(vShader);
	SSAO_SX.addShader(fShader);
	SSAO_SX.link();
	if (!SSAO_SX.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << SSAO_SX.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
	////////////////////////////////////////
	vShader.initFromFile(VERTEX_SHADER, "shaders/SSAO-SY.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/SSAO-SY.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	SSAO_SY.init();
	SSAO_SY.addShader(vShader);
	SSAO_SY.addShader(fShader);
	SSAO_SY.link();
	if (!SSAO_SY.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << SSAO_SY.log() << endl << endl;
	}
	vShader.free();
	fShader.free();
///////////////////////////////////////////

	vShader.initFromFile(VERTEX_SHADER, "shaders/SSAO-SFinal.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/SSAO-SFinal.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	SSAO_SFinal.init();
	SSAO_SFinal.addShader(vShader);
	SSAO_SFinal.addShader(fShader);
	SSAO_SFinal.link();
	if (!SSAO_SFinal.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << SSAO_SFinal.log() << endl << endl;
	}
	vShader.free();
	fShader.free();



}


