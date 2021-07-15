#include "Mesh.h"
#include <iostream>

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

/**
*	Constructor, loading the specified aiMesh
**/
Mesh::MeshEntry::MeshEntry(aiMesh *mesh, ShaderProgram &program , glm::mat4 loc1) {
	 
	prog = program;
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;
	
	loc = loc1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	 
	 
		
	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		float *vertices = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}
		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		posLocation = program.bindVertexAttribute("position", 3, 3 * sizeof(GLfloat), 0);

		 

		delete[] vertices;
	}
	 

		if (mesh->HasNormals()) {
			float *normals = new float[mesh->mNumVertices * 3];
			for (int i = 0; i < mesh->mNumVertices; ++i) {
				normals[i * 3] = mesh->mNormals[i].x;
				normals[i * 3 + 1] = mesh->mNormals[i].y;
				normals[i * 3 + 2] = mesh->mNormals[i].z;
			}
			glGenBuffers(1, &vbo[NORMAL_BUFFER]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);
			normalLocation = program.bindVertexAttribute("normal", 3, 3 * sizeof(GLfloat), 0);
			 delete[] normals;


		}


		if (mesh->HasFaces()) {
			unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
			for (int i = 0; i < mesh->mNumFaces; ++i) {
				indices[i * 3] = mesh->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			 
			}
			glGenBuffers(1, &vbo[INDEX_BUFFER]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);
 

			delete[] indices;
			
		};

		 
		draw_bbox(mesh, program);
}

  
Mesh::MeshEntry::~MeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &   vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, & vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &  vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &  vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &  vao);
}

/**
*	Renders this MeshEntry
**/

void Mesh::MeshEntry::draw_bbox(aiMesh *mesh, ShaderProgram &program)
{
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	if (mesh->mNumVertices == 0)
		return;

	GLfloat vertice[] =
	{
		  -0.5,-0.5f,-0.5f, // triangle 1 : begin
		  -0.5,-0.5f, 0.5f,
		  -0.5, 0.5f, 0.5f, // triangle 1 : end
		   0.5, 0.5f,-0.5f, // triangle 2 : begin
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f,-0.5f, // triangle 2 : end
		   0.5,-0.5f, 0.5f,
		  -0.5,-0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		  -0.5, 0.5f,-0.5f,
		   0.5,-0.5f, 0.5f,
		  -0.5,-0.5f, 0.5f,
		  -0.5,-0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		  -0.5,-0.5f, 0.5f,
		   0.5,-0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f,-0.5f,
		   0.5,-0.5f,-0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5,-0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		   0.5, 0.5f,-0.5f,
		  -0.5, 0.5f,-0.5f,
	       0.5, 0.5f, 0.5f,
		  -0.5, 0.5f,-0.5f,
		  -0.5, 0.5f, 0.5f,
		   0.5, 0.5f, 0.5f,
		  -0.5, 0.5f, 0.5f,
		   0.5,-0.5f, 0.5f
	};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice), vertice, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 3, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(posLocation);

 
	 
	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;
		min_x = max_x = mesh->mVertices[0].x;
		min_y = max_y = mesh->mVertices[0].y;
		min_z = max_z = mesh->mVertices[0].z;
		for (int i = 0; i <  mesh->mNumVertices; i++) {
			if (mesh->mVertices[i].x < min_x) min_x = mesh->mVertices[i].x;
			if (mesh->mVertices[i].x > max_x) max_x = mesh->mVertices[i].x;
			if (mesh->mVertices[i].y < min_y) min_y = mesh->mVertices[i].y;
			if (mesh->mVertices[i].y > max_y) max_y = mesh->mVertices[i].y;
			if (mesh->mVertices[i].z < min_z) min_z = mesh->mVertices[i].z;
			if (mesh->mVertices[i].z > max_z) max_z = mesh->mVertices[i].z;
	}
		
	glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
	glm::vec3 center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);
	 
    calcBox = transform;
	 
	 
}

void Mesh::MeshEntry::render() {
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(normalLocation);
	 
 	
	

	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);

	 
	}
void Mesh::MeshEntry::renderBox() {
	glBindVertexArray(VAO1);
	
	glm::mat4 transform =(  loc*calcBox)  ;
	prog.setUniformMatrix4f("model_mat", transform);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	 
}
 
Mesh::Mesh(const char *filename , ShaderProgram &program ,glm::mat4 p)
{
	loc1 = p;
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate|aiProcess_FlipUVs);
	if (!scene) {
		printf("Unable to laod mesh: %s\n", importer.GetErrorString());
	}

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		meshEntries.push_back(new Mesh::MeshEntry(scene->mMeshes[i],  program, loc1));
	}
}

 
Mesh::~Mesh(void)
{
	for (int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}
 
void Mesh::render(   ) {
	for (int i = 0; i < meshEntries.size(); ++i) {
		meshEntries.at(i)->render( );
		 
	}
}
void Mesh::renderb() {
	for (int i = 0; i < meshEntries.size(); ++i) {
		 
		meshEntries.at(i)->renderBox();
	}
}

   