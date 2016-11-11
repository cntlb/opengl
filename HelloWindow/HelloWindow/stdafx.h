// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件
// GLEW
//注意，之所以定义GLEW_STATIC宏，是因为我们使用的是GLEW静态的链接库。
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>