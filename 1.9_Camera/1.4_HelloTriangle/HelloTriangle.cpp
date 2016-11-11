// HelloWindow.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"

using namespace std;

static float rotateX = -55.0f;
static float translateX;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//存储按键按下的状态
bool keystatus[1024];
//键盘按键回调事件
void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

/*
自定义looAt矩阵
@param eye: camera position
@param center: camera focus center
@param eye: world coordinate up
*/
glm::mat4 my_looAt_matrix(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
	glm::mat4 translate;
	translate[0][3] = -eye.x;
	translate[1][3] = -eye.y;
	translate[2][3] = -eye.z;
	//glm中是列主序,因此需要转置
	translate = glm::transpose(translate);

	glm::vec3 newdir = glm::normalize(eye - center);
	glm::vec3 newright = glm::normalize(glm::cross(newdir, up));
	glm::vec3 newup = glm::cross(newright, newdir);
	glm::vec3 zero = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 rotate = glm::mat4(
		glm::vec4(newright,	0.0f),
		glm::vec4(newup,	0.0f),
		glm::vec4(newdir,	0.0f),
		glm::vec4(zero,		1.0f)
	);
	rotate = glm::transpose(rotate);

	return rotate*translate;
}
void do_movment();

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

	//GLfloat vertices[] = {
	//	// ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	//};

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

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
		do_movment();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 启用深度测试
		glEnable(GL_DEPTH_TEST);

		shader.use();

		//绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader.program, "ourTexture0"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shader.program, "ourTexture1"), 1);

		//传入变换矩阵
		//glm::mat4 trans;
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.program, "transf"), 1, GL_FALSE, glm::value_ptr(trans));
		
		////3D变换矩阵
		//glm::mat4 model;
		////model = glm::rotate(model, rotateX, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 1.0f, 0.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		//glm::mat4 view;
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


		//GLfloat radius = 10.0f;
		//GLfloat camX = sin(glfwGetTime()) * radius;
		//GLfloat camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		//glm::vec3 director = glm::vec3(-camX, 0.0f, -camZ);
		//glm::vec3 right = glm::cross(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 1.0, 0.0));
		//glm::vec3 up = glm::cross(right, glm::vec3(camX, 0.0, camZ));
		//view = glm::mat4(glm::vec4(glm::normalize(right), 0.0f),
		//				 glm::vec4(glm::normalize(up), 0.0f),
		//				 glm::vec4(glm::normalize(director), 0.0f),
		//				 glm::vec4(0.0f,0.0f,0.0f, 1.0f));
		//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		view = my_looAt_matrix(cameraPos, cameraPos + cameraFront, cameraUp);

		glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection;
		projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			//GLfloat angle = (GLfloat)glfwGetTime() * 50.0f;
			model = glm::rotate(model, rotateX, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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

	if (action == GLFW_PRESS) {
		keystatus[key] = true;
	}
	if (action == GLFW_RELEASE) {
		keystatus[key] = false;
	}

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			exit(0);
			break;
		}
	}

}

void do_movment() {
	GLfloat cameraSpeed = 0.05f;
	if (keystatus[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keystatus[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keystatus[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keystatus[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keystatus[GLFW_KEY_UP])
		rotateX -= 0.1f;
	if (keystatus[GLFW_KEY_DOWN])
		rotateX += 5.0f;
}
