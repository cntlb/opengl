// HelloWindow.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "1.4_HelloTriangle.h"

using namespace std;

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
	//编译着色器
	string sourceCode;
	Utils::read(path, sourceCode);
	cout << sourceCode.c_str() << endl;
	shader = glCreateShader(type);
	const GLchar * source[1];
	source[0] = sourceCode.c_str();
	GLint size = sourceCode.length();
	glShaderSource(shader, 1, source, &size);
	glCompileShader(shader);
	//检测编译错误
	checkShaderCompile(shader);
}

void createVAOandVBO(GLuint &VAO, GLuint &VBO, GLuint size, void*data) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

int main()
{
	//初始化窗口
	glfwInit();

	//配置glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);//窗口附着在当前线程

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//配置视口
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	//按键回调
	glfwSetKeyCallback(window, onKeyEvent);

	//=============triangle============
	//编译着色器
	GLuint vertexShader;
	compileShader(vertexShader, "shader.vertex", GL_VERTEX_SHADER);
	GLuint colorShader, blueShader;
	compileShader(colorShader, "color.fragment", GL_FRAGMENT_SHADER);
	compileShader(blueShader, "color-blue.fragment", GL_FRAGMENT_SHADER);
	//链接着色器
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, colorShader);
	glLinkProgram(shaderProgram);
	checkProgramLink(shaderProgram);

	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, blueShader);
	glLinkProgram(shaderProgram2);
	checkProgramLink(shaderProgram2);
	//把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(vertexShader);
	glDeleteShader(colorShader);
	glDeleteShader(blueShader);

	GLfloat vertices1[] = {
		//first 
		-0.8f, -0.5f, 0.0f,
		0.2f, -0.5f, 0.0f,
		-0.3f,  0.5f, 0.0f,
	};
	GLfloat vertices2[] = {
		//second
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
		1.0f,  0.5f, 0.0f,
	};
	//创建VBO
	GLuint VAO1,VBO1, VAO2, VBO2;
	createVAOandVBO(VAO1, VBO1, sizeof(vertices1), vertices1);
	createVAOandVBO(VAO2, VBO2, sizeof(vertices2), vertices2);
		
	while (!glfwWindowShouldClose(window))
	{
		// 检查事件
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// 交换缓冲
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

