#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE



#include <glm/glm.hpp>
#include "Camera.h"
#include "ShaderProgram.h"
#include "TriangleMesh.h"
#include "Text.h"
#include "Mesh.h"
#include <random>

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	bool loadMesh(const char *filename);
	void update(int deltaTime);
	void render();
	void   BuildScene();

  Camera &getCamera();
  public: int n; 
  void switchPolygonMode();
  void switchOQMode();
  bool  noramlSSAO = true;
  float radius = 0.11f;
  int intentsity = 1;
  int kernelSize = 128;

private:
	void initShaders();
	void computeModelViewMatrix();
	void renderQuad();
	void renderQuad(GLint tex);

private:unsigned int quadVAO = 0;
		unsigned int quadVBO;
		unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
		unsigned int ssao_sx_ColorBuffer, ssao_sy_ColorBuffer, ssao_sFinal_ColorBuffer;

	unsigned int gBuffer;
	unsigned int ssaoFBO, ssaoBlurFBO;
	unsigned int ssao_SX_FBO, ssao_SY_FBO, ssao_SFinal_FBO;

	unsigned int gPosition, gNormal, gAlbedo;
	unsigned int noiseTexture;
	unsigned int rboDepth;
	//std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0

	default_random_engine generator;
	//	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
	glm::vec3 lightColor = glm::vec3(.2 ,.2,.7);

	vector<glm::vec3> ssaoKernel;
  Camera camera;
	TriangleMesh *mesh;
	ShaderProgram basicProgram;
	ShaderProgram Geometry;
	ShaderProgram SSAO;
	ShaderProgram QuadSahder;
	ShaderProgram LightShader;
	ShaderProgram BlurShader;
	ShaderProgram SSAO_SX;
	ShaderProgram SSAO_SY;
	ShaderProgram SSAO_SFinal;



	float currentTime;
	int obnum;
	bool bPolygonFill;
	  
     
	  Mesh* object;
	  Mesh* object2;
	  Mesh* floor;

      Mesh *tile ;
  //  Mesh *wall;
	  Text text;
	 // glm::mat4 locs[1000];
	  glm::mat4 locs;
	  glm::mat4 locs2;
	  glm::mat4 locs3;

};


#endif // _SCENE_INCLUDE

