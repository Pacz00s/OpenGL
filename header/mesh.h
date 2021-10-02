#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


//struct for vertex with uv
struct Vertex3dUVNormal
{

    glm::vec3 m_position;
    glm::vec2 m_texCoord;
    glm::vec3 m_normal;

    // Makes a 2d vertex with uc and color data.
    Vertex3dUVNormal(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal) {
        m_position = position;
        m_texCoord = texCoord;
        m_normal = normal;
    }

	Vertex3dUVNormal() {}
};

class Mesh {


private:
	// Vectors of shape information
	std::vector<Vertex3dUVNormal> m_vertices;
	std::vector<unsigned short> m_indices;

	// Buffered shape info
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;


public:
	// Constructor for a shape, takes a vector for vertices and indices
	Mesh(std::vector<Vertex3dUVNormal> vertices, std::vector<unsigned short> indices);

    // Constructor for a mesh. reads in an obj file.
    Mesh(std::string filePath);

	// Shape destructor to clean up buffers
	~Mesh();


	// Draws the shape using a given world matrix
	void Draw();
};