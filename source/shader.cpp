#include "../header/shader.h"
#include <string.h>
#include <errno.h>
Shader::Shader(std::string sfilePath, GLenum shaderType)
{
     m_shader = 0;
	 m_type = 0;;
     m_refCount = 0;;


    std::fstream _file;
    _file.open(sfilePath,std::fstream::in | std::fstream::out | std::fstream::app);

    if (!_file.good())
    {
        std::cout << "Nie znaleziono sam wiesz czego: " << sfilePath << std::endl;
    }
    else
    {


	// ifstream internally keeps track of where in the file.

	// Here we find the end of the file.
	_file.seekg(0, std::ios::end);

	// Make a string and set its size equal to the length of the file.
	std::string shaderCode;
	shaderCode.resize((size_t)_file.tellg());

	// Go back to the beginning of the file.
	_file.seekg(0, std::ios::beg);

	// Read the file into the string until we reach the end of the string.
	_file.read(&shaderCode[0], shaderCode.size());

	// Close the file.
	_file.close();

	// Init using the string.
	InitFromString(shaderCode, shaderType);
	}
}

Shader::~Shader()
{
	// Only delete the shader index if it was initialized successfully.
	if (m_shader != 0)
	{
		glDeleteShader(m_shader);
	}
}

GLuint Shader::GetGLShader()
{
    return m_shader;
}

GLenum Shader::GetGLShaderType()
{
    return m_type;
}

/*bool Shader::InitFromFile(std::string filePath, GLenum shaderType)
{

    /*std::ifstream _file(filePath);

    if (!_file.good())
    {
        std::cout << "Nie znaleziono modelu: " << filePath << std::endl;
        exit(666);
    }

	// Check if the file exists
	if (_file.is_open() == false)
	{
		// If we encounter an error, print a message and return false.
		std::cerr << "Nie odczytano pliku shjdera: " << filePath << std::endl;

	}

	// ifstream internally keeps track of where in the file.

	// Here we find the end of the file.
	_file.seekg(0, std::ios::end);

	// Make a string and set its size equal to the length of the file.
	std::string shaderCode;
	shaderCode.resize((size_t)_file.tellg());

	// Go back to the beginning of the file.
	_file.seekg(0, std::ios::beg);

	// Read the file into the string until we reach the end of the string.
	_file.read(&shaderCode[0], shaderCode.size());

	// Close the file.
	_file.close();

	// Init using the string.
	return InitFromString(shaderCode, shaderType);

	exit(22);
}
*/
bool Shader::InitFromString(std::string shaderCode, GLenum shaderType)
{
	m_type = shaderType;
	m_shader = glCreateShader(shaderType);

	// Get the char* and length
	const char* shaderCodePointer = shaderCode.data();
	int shaderCodeLength = shaderCode.size();

	// Set the source code and compile.
	glShaderSource(m_shader, 1, &shaderCodePointer, &shaderCodeLength);
	glCompileShader(m_shader);

	GLint isCompiled;

	// Check if the fragmentShader compiles:
	// If it failed, print an error and delete it.
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled)
	{
		char infolog[1024];
		glGetShaderInfoLog(m_shader, 1024, NULL, infolog);
		std::cout << "Blad kompilacji shajdera: " << std::endl << infolog << std::endl;

		// Delete the shader, and set the index to zero so that this object knows it doesn't have a shader.
		glDeleteShader(m_shader);
		m_shader = 0;
		return false;
	}
	else
	{
		return true;
	}
}

void Shader::IncRefCount()
{
    m_refCount++;
}

void Shader::DecRefCount()
{
    m_refCount--;
    if (m_refCount == 0)
    {
        delete this;
    }
}
