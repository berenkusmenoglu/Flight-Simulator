#pragma once

#include <string>
#include "../gl/glew.h"
#include <windows.h>

// GLSL wrapper
class Shader
{
public:
	Shader(void);
	~Shader(void);

private:
	Shader(const Shader&);
	Shader& operator=(const Shader&);

private:
	std::string m_name;
	GLuint m_vertexShader;       //identifier for the vertex shader
	GLuint m_fragmentShader;     //identifier for the fragment shader
	GLuint m_programObject;      //identifier for the program- this is used when rendering.

public:
	//returns what we need for rendering
	GLuint handle(void) const { return m_programObject; } 

	//loads the shader program from two text files
	bool load(const std::string name, const char* vertexFilename, const char* fragmentFilename);
	
private:
	GLuint loadShader(const char* filename, const GLenum type) const;
	std::string shaderInfoLog(const GLuint shader) const;
	std::string programInfoLog(const GLuint program) const;
};