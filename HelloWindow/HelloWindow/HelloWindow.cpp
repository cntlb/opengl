// HelloWindow.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"

using namespace std;

void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} 
}

int main()
{
	//��ʼ������
	glfwInit();

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

	//glfwWindowShouldClose����������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳���
	//	����ǵĻ��ú�������trueȻ����Ϸѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�
	//glfwPollEvents���������û�д���ʲô�¼�������������롢����ƶ��ȣ���
	//	Ȼ����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�������һ������Ϸѭ���Ŀ�ʼ�����¼���������
	//glfwSwapBuffers�����ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫ�Ĵ󻺳壩��
	//	������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
	while (!glfwWindowShouldClose(window))
	{
		// ����¼�
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ��������
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

