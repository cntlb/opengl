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

int main()
{
	//初始化窗口
	glfwInit();

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
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	//创建VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	
	//编译着色器
	GLuint vertexShader;
	compileShader(vertexShader, "shader.vertex", GL_VERTEX_SHADER);
	GLuint colorShader;
	compileShader(colorShader, "color.fragment", GL_FRAGMENT_SHADER);
	//链接着色器
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, colorShader);
	glLinkProgram(shaderProgram);
	checkProgramLink(shaderProgram);

	//链接顶点属性
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//顶点数据复制到缓冲的内存中：
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//使用着色器程序
	glUseProgram(shaderProgram);
	//把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(vertexShader);
	glDeleteShader(colorShader);
		
	/**
	glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，
		如果是的话该函数返回true然后游戏循环便结束了，之后为我们就可以关闭应用程序了。
	glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），
		然后调用对应的回调函数（可以通过回调方法手动设置）。我们一般在游戏循环的开始调用事件处理函数。
	glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲），
		它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
	*/
	while (!glfwWindowShouldClose(window))
	{
		// 检查事件
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// 交换缓冲
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

