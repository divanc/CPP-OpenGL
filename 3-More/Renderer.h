//
//  Renderer.h
//  3-More
//
//  Created by Ivan on 10.06.2022.
//
#pragma once

#include <glad/glad.h>


#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
