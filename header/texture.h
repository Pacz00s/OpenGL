#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "FreeImage.h"
#include <iostream>

class Texture
{
private:
    GLuint m_texture;
    unsigned int m_refCount = 0;

public:
    Texture(char* filePath);
    ~Texture();
    void IncRefCount();
    void DecRefCount();
    GLuint GetGLTexture();

};