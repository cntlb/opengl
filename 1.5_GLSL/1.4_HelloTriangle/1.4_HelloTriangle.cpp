// HelloWindow.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"


using namespace std;
//���̰����ص��¼�
void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

//������ɫ��
void compileShader(GLuint &shader, const string path, GLenum type);

//�����ɫ���������
void checkShaderCompile(GLuint shader);

//�����ɫ���������Ӵ���
void checkProgramLink(GLuint programe);



int main()
{
	//��ʼ������
	glfwInit();

	//GLint nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	//glfw �汾��Ϣ
	int major, minor, ret;
	glfwGetVersion(&major, &minor, &ret);
	cout << major << endl << minor << endl << ret << endl;
	cout << glfwGetVersionString() << endl;

	//����glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//���ڸ����ڵ�ǰ�߳�

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//�����ӿ�
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//�����ص�
	glfwSetKeyCallback(window, onKeyEvent);

	//=============triangle============
	
	
	////������ɫ��
	//GLuint vertexShader;
	//compileShader(vertexShader, "shader.vertex", GL_VERTEX_SHADER);
	//GLuint colorShader;
	//compileShader(colorShader, "color.fragment", GL_FRAGMENT_SHADER);
	////������ɫ��
	//GLuint shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, colorShader);
	//glLinkProgram(shaderProgram);
	//checkProgramLink(shaderProgram);
	////����ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ�����ˣ�
	//glDeleteShader(vertexShader);
	//glDeleteShader(colorShader);

	//ʹ���µ���ɫ������
	Shader shader("shader.vertex", "color.fragment");


	//���Ӷ�������
	/*GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};*/

	GLfloat vertices[] = {
		// λ��              // ��ɫ
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f    // ����
	};
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//GLint location = glGetUniformLocation(shader.program, "ourColor");
		
	while (!glfwWindowShouldClose(window))
	{
		// ����¼�
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangle
		//GLfloat time = glfwGetTime();
		//GLfloat green = sin(time) / 2 + 0.5;
		//����һ��unform֮ǰ�������ʹ�ó���
		//�����Ѿ�������һ��glUseProgram�� �ʴ˴����Բ�����˳��
		//����ס������⣬���Ǻõ�
		//glUseProgram(shaderProgram);
		//glUniform4f(location, 0.0f, green, 0.0f, 1.0f);
		shader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// ��������
		glfwSwapBuffers(window);
	}
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void checkShaderCompile(GLuint shader) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void checkProgramLink(GLuint programe) {
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(programe, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programe, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAME::LINK_FAILED\n" << infoLog << std::endl;
	}
}

void compileShader(GLuint &shader, const string path, GLenum type) {
	//������ɫ��
	string sourceCode;
	Utils::read(path, sourceCode);
	cout << sourceCode.c_str() << endl;
	shader = glCreateShader(type);
	const GLchar * source[1];
	source[0] = sourceCode.c_str();
	GLint size = sourceCode.length();
	glShaderSource(shader, 1, source, &size);
	glCompileShader(shader);
	//���������
	checkShaderCompile(shader);
}