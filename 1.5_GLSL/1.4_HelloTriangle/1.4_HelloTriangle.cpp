// HelloWindow.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"


using namespace std;
//键盘按键回调事件
void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

//编译着色器
void compileShader(GLuint &shader, const string path, GLenum type);

//检测着色器编译错误
void checkShaderCompile(GLuint shader);

//检测着色器程序链接错误
void checkProgramLink(GLuint programe);



int main()
{
	//初始化窗口
	glfwInit();

	//GLint nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	//glfw 版本信息
	int major, minor, ret;
	glfwGetVersion(&major, &minor, &ret);
	cout << major << endl << minor << endl << ret << endl;
	cout << glfwGetVersionString() << endl;

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
	
	
	////编译着色器
	//GLuint vertexShader;
	//compileShader(vertexShader, "shader.vertex", GL_VERTEX_SHADER);
	//GLuint colorShader;
	//compileShader(colorShader, "color.fragment", GL_FRAGMENT_SHADER);
	////链接着色器
	//GLuint shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, colorShader);
	//glLinkProgram(shaderProgram);
	//checkProgramLink(shaderProgram);
	////把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	//glDeleteShader(vertexShader);
	//glDeleteShader(colorShader);

	//使用新的着色器程序
	Shader shader("shader.vertex", "color.fragment");


	//链接顶点属性
	/*GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};*/

	GLfloat vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f    // 顶部
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
		// 检查事件
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangle
		//GLfloat time = glfwGetTime();
		//GLfloat green = sin(time) / 2 + 0.5;
		//更新一个unform之前你必须先使用程序
		//上面已经调用了一次glUseProgram， 故此处可以不考虑顺序
		//但记住这个问题，总是好的
		//glUseProgram(shaderProgram);
		//glUniform4f(location, 0.0f, green, 0.0f, 1.0f);
		shader.use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// 交换缓冲
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