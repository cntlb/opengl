#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader(const char* vertexFile, const char* fragmentFile) {

	GLint success;
	GLchar infoLog[512];
	auto f = [&](const char* sourceFile, GLenum type)->GLuint {

		ifstream input;
		input.exceptions(ifstream::badbit);
		input.open(sourceFile);
		stringstream vertex_stream;
		vertex_stream << input.rdbuf();
		string vertex_source = vertex_stream.str();
		cout << vertex_source << endl;

		GLuint shader = glCreateShader(type);
		const GLchar *sourceCode = vertex_source.c_str();
		glShaderSource(shader, 1, &sourceCode, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		return shader;
	};

	GLuint vertexShader = f(vertexFile, GL_VERTEX_SHADER);
	GLuint fragmentShader = f(fragmentFile, GL_FRAGMENT_SHADER);

	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);
	glLinkProgram(this->program);


	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAME::LINK_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::use() {
	glUseProgram(this->program);
}