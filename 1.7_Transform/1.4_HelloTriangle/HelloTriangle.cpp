// HelloWindow.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"

using namespace std;

//键盘按键回调事件
void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

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

	//着色器
	Shader shader("shader.vertex", "color.fragment");

	//纹理
	auto loadTexture = [&](const char* path) {
		unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
		if (image == NULL) {
			cout << "load image error" << endl;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	};

	GLuint texture1 = loadTexture("wall.jpg");
	GLuint texture2 = loadTexture("face.png");

	//链接顶点属性
	/*GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};*/

	//GLfloat vertices[] = {
	//	// 位置              // 颜色
	//	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // 右下
	//	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	//	0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f    // 顶部
	//};

	GLfloat vertices[] = {
		// ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	//glm库进行变换
	glm::mat4 transf;
	transf = glm::rotate(transf, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	transf = glm::scale(transf, glm::vec3(0.5f, 0.5f, 1.0f));

	//GLint location = glGetUniformLocation(shader.program, "ourColor");
		
	while (!glfwWindowShouldClose(window))
	{
		// 检查事件
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		//绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader.program, "ourTexture0"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shader.program, "ourTexture1"), 1);

		//传入变换矩阵
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "transf"), 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
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
