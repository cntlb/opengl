[TOC]

## 1. 入门
### 1.1 OpenGL

一般它被认为是一个API(Application Programming Interface, 应用程序编程接口)，包含了一系列可以操作图形、图像的函数。然而，OpenGL本身并不是一个API，它仅仅是一个由[Khronos组织](http://www.khronos.org/)制定并维护的规范(Specification)。

从OpenGL3.2开始，规范文档开始废弃立即渲染模式，推出核心模式(Core-profile)，这个模式完全移除了旧的特性。所有OpenGL的更高的版本都是在3.3的基础上，引入了额外的功能，并没有改动核心架构。新版本只是引入了一些更有效率或更有用的方式去完成同样的功能。

**扩展**

使用扩展的代码大多看上去如下：

```c++
  if(GL_ARB_extension_name)
  {
      // 使用一些新的特性
  }
  else
  {
      // 不支持此扩展: 用旧的方式去做
  }
```

OpenGL的一大特性就是对扩展(Extension)的支持，当一个显卡公司提出一个新特性或者渲染上的大优化，通常会以扩展的方式在驱动中实现。

OpenGL自身是一个巨大的**状态机**(State Machine)：一系列的变量描述OpenGL此刻应当如何运行。

**基元类型(Primitive Type)**

使用OpenGL时，建议使用OpenGL定义的基元类型。比如使用`float`时我们加上前缀`GL`（因此写作`GLfloat`）。`int`、`uint`、`char`、`bool`等等也类似。OpenGL定义的这些GL基元类型的内存布局是与平台无关的，而int等基元类型在不同操作系统上可能有不同的内存布局。使用GL基元类型可以保证你的程序在不同的平台上工作一致。



### 1.2 创建窗口

* GLFW可以从它官方网站的[下载页](http://www.glfw.org/download.html)上获取。

* CMake是一个工程文件生成工具。用户可以使用预定义好的CMake脚本，根据自己的选择（像是Visual Studio, Code::Blocks, Eclipse）生成不同IDE的工程文件。从[这里](http://www.cmake.org/cmake/resources/software.html)下载安装CMake。

* GLEW是OpenGL Extension Wrangler Library的缩写, 可以从[这里](http://glew.sourceforge.net/index.html)下载

  ​

**第一个工程**
*  打开Visual Studio，创建一个新的项目。如果VS提供了多个选项，选择Visual C++，然后选择Empty Project(空项目)

*  **VC++ Directories(VC++ 目录)**添加**Include Directories(包含目录)**和**Library Directories(包含目录)**

*  **Linker(链接器)**选项卡里**的Input(输入)**选项卡里添加**glfw3.lib, glwe32s.lib**

*  静态链接GLEW，必须在包含GLEW头文件之前定义预处理器宏`GLEW_STATIC`：

   ```c++
            #define GLEW_STATIC
            #include <GL/glew.h>
   ```


### 1.3 Hello Window

```c++
//注意，之所以定义GLEW_STATIC宏，是因为我们使用的是GLEW静态的链接库。
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} 
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

	//为调用 GLFW的函数创建GLFWwindow对象
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

	//glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，
	//	如果是的话该函数返回true然后游戏循环便结束了，之后为我们就可以关闭应用程序了。
	//glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），
	//	然后调用对应的回调函数（可以通过回调方法手动设置）。我们一般在游戏循环的开始调用事件处理函数。
	//glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲），
	//	它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
	while (!glfwWindowShouldClose(window))
	{
		// 检查事件
		glfwPollEvents();

      	// 渲染指令 ...
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 交换缓冲
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
```

### 1.3 Hello Triangle

OpenGL着色器是用OpenGL着色器语言(OpenGL Shading Language, GLSL)写成的它。下面是一个图形渲染管线的每个阶段的抽象展示。要注意蓝色部分代表的是我们可以注入自定义的着色器的部分。
![img](https://learnopengl-cn.github.io/img/01/04/pipeline.png)

* 顶点输入

  ```c++
  GLfloat vertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
  };
  ```

  **标准化设备坐标(Normalized Device Coordinates, NDC)**

  一旦你的顶点坐标已经在顶点着色器中处理过，它们就应该是**标准化设备坐标**了，标准化设备坐标是一个x、y和z值在-1.0到1.0的一小段空间。任何落在范围外的坐标都会被丢弃/裁剪，不会显示在你的屏幕上。下面你会看到我们定义的在标准化设备坐标中的三角形(忽略z轴)：

  ![NDC](https://learnopengl-cn.github.io/img/01/04/ndc.png)

  与通常的屏幕坐标不同，y轴正方向为向上，(0, 0)坐标是这个图像的中心，而不是左上角。最终你希望所有(变换过的)坐标都在这个坐标空间中，否则它们就不可见了。

  你的标准化设备坐标接着会变换为屏幕空间坐标(Screen-space Coordinates)，这是使用你通过glViewport函数提供的数据，进行视口变换(Viewport Transform)完成的。所得的屏幕空间坐标又会被变换为片段输入到片段着色器中。

* 顶点缓冲对象(Vertex Buffer Objects, VBO)

  ```c++
  GLuint VBO;
  //创建VBO
  glGenBuffers(1, &VBO);  
  //将缓冲绑定到GL_ARRAY_BUFFER上
  glBindBuffer(GL_ARRAY_BUFFER, VBO);  
  //将顶点数据拷贝到缓冲区
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  ```

* 顶点着色器(Vertex Shader)是几个可编程着色器中的一个

  用着色器语言GLSL(OpenGL Shading Language)编写顶点着色器，然后编译这个着色器:

  ```
  #version 330 core

  layout (location = 0) in vec3 position;

  void main()
  {
      gl_Position = vec4(position.x, position.y, position.z, 1.0);
  }
  ```

* 编译着色器

  ```c++
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  ```

* 着色器编译错误信息

  ```c++
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  ```

* 片段着色器

  ```c++
  #version 330 core

  out vec4 color;

  void main()
  {
      color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
  }
  ```

* 着色程序

  ```c++
  //创建程序
  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  //把之前编译的着色器附加到程序对象上
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  //链接程序
  glLinkProgram(shaderProgram);

  //着色器对象链接到程序对象以后，记得删除着色器对象
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  ```

* 着色程序错误检查

  ```c++
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    ...
  }
  ```

* 链接顶点属性

  ![img](https://learnopengl-cn.github.io/img/01/04/vertex_attribute_pointer.png)

  ```c++
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  ```

* 顶点数组对象(Vertex Array Object, VAO)

  ![img](https://learnopengl-cn.github.io/img/01/04/vertex_array_objects.png)

  ```c++
      GLuint VAO;
      glGenVertexArrays(1, &VAO);  

      / ..:: 初始化代码（只运行一次 (除非你的物体频繁改变)） :: ..
      // 1. 绑定VAO
      glBindVertexArray(VAO);
      // 2. 把顶点数组复制到缓冲中供OpenGL使用
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      // 3. 设置顶点属性指针
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);
      //4. 解绑VAO
      glBindVertexArray(0);

      [...]

      // ..:: 绘制代（游戏循环中） :: ..
      // 5. 绘制物体
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      someOpenGLFunctionThatDrawsOurTriangle();
      glBindVertexArray(0);
  ```

* 索引缓冲对象(EBO), 和VBO创建绑定类似
