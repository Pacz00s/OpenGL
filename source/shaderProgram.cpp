#include "../header/shaderProgram.h"

ShaderProgram::ShaderProgram()
{
    m_shaderProgram = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_shaderProgram);

    // zmniejsz iloœc shaderów je¿eli obiekt jest skasowany
    if (m_vertexShader != nullptr)
        m_vertexShader->DecRefCount();

    if (m_fragmentShader != nullptr)
        m_fragmentShader->DecRefCount();
}

GLuint ShaderProgram::GetGLShaderProgram()
{
    return m_shaderProgram;
}

void ShaderProgram::AttachShader(Shader* shader)
{
    // sprawdzenie typów shajerdrów
    Shader** currentShader;
    switch (shader->GetGLShaderType())
    {
        case GL_VERTEX_SHADER:
            currentShader = &m_vertexShader;
            break;
        case GL_FRAGMENT_SHADER:
            currentShader = &m_fragmentShader;
            break;
        default:
            return;
    }
    // zwiêksz iloœc shaderów
    shader->IncRefCount();

    // Check if a different shader already lives there if it does, decrement it.
    if (*currentShader != nullptr)
        (*currentShader)->DecRefCount();

    // Replace it with the new shader
    *currentShader = shader;

    // Attach the gl shader to the shader program.
    if (shader->GetGLShader() != 0)
    {
        glAttachShader(m_shaderProgram, shader->GetGLShader());
        // ShaderProgram must be rebuilt
        m_programBuilt = false;
    }
    else
    {
        // Print an error if trying to attach an uninitialized shader.
        std::cout << "Nie mozna do³¹czyæ shejdera." << std::endl;
    }
}

void ShaderProgram::Bind()
{
    if (!m_programBuilt)
    {
        //
        glLinkProgram(m_shaderProgram);
        m_programBuilt = true;
    }

    glUseProgram(m_shaderProgram);
}

void ShaderProgram::Unbind()
{
    glUseProgram(0);
}

void ShaderProgram::IncRefCount()
{
    m_refCount++;
}

void ShaderProgram::DecRefCount()
{
    m_refCount--;
    if (m_refCount == 0)
    {
        delete this;
    }
}
