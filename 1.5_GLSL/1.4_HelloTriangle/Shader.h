#pragma once

#ifndef SHADER_H
#define SHADER_H
#include <GL\glew.h>
class Shader {
public:
	Shader(const char* vertexFile, const char* fragmentFile);

	void use();

public:
	GLuint program;
};

#endif
