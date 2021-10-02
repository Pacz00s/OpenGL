#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>
#include <fstream>

class Shader
{

private:
	GLuint m_shader;
	GLenum m_type;

    // Reference Counter
    unsigned int m_refCount;

public:
	Shader(std::string filePath, GLenum shaderType);
	~Shader();

    GLuint GetGLShader();
    GLenum GetGLShaderType();

	//bool InitFromFile(std::string, GLenum shaderType);
	bool InitFromString(std::string shaderCode, GLenum shaderType);

    void IncRefCount();
    void DecRefCount();
};
