#pragma once

#include "GL/glew.h"

#include "assimp\scene.h"
#include "assimp\mesh.h"
#include "ShaderProgram.h"

#include  <vector>

class Mesh
{
public:


 
	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];
		GLuint vbo_vertices;
		GLuint ibo_elements;
		GLuint VAO1;
		ShaderProgram prog;
		unsigned int elementCount;
		GLint posLocation, normalLocation, posL;

		MeshEntry(aiMesh *mesh, ShaderProgram &program, glm::mat4 loc);
		~MeshEntry();
		glm::mat4 calcBox;
		glm::mat4 loc;

		void load(aiMesh *mesh);
		void render(    );
		void renderBox();
		void draw_bbox(aiMesh *mesh , ShaderProgram &program);
	};

	std::vector<MeshEntry*> meshEntries;
	 
public:
	  aiMesh MEsh;
	  glm::mat4 loc1 ;
	 
	void Mesh::draw_bbox(Mesh* mesh);
	Mesh(const char *filename, ShaderProgram &program ,glm::mat4 p);
	~Mesh(void);

//	unsigned int  indices;
	void render();
	void renderb();
};