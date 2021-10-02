#pragma once
#include "shader.h"
#include <iostream>

// Wraps opengl shader program functionality
class ShaderProgram
{

private:
    // These shader objects wrap the functionality of loading and compiling shaders from files.
    Shader* m_vertexShader = nullptr;
    Shader* m_fragmentShader = nullptr;

    // GL index for shader program
    GLuint m_shaderProgram;

    // Keep track of if the program has been built and only build when needed
    bool m_programBuilt = false;

    // Reference Counter
    unsigned int m_refCount = 0;

public:
    ShaderProgram();
    ~ShaderProgram();
    GLuint GetGLShaderProgram();
    void AttachShader(Shader* shader);
    void Bind();
    void Unbind();
    void IncRefCount();
    void DecRefCount();
};