//
//  Errors.h
//  2-Square
//
//  Created by Ivan on 09.06.2022.
//

#ifndef Errors_h
#define Errors_h


#endif /* Errors_h */


#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __builtin_trap();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR) {}
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    bool hasError = false;
    
    while (GLenum error = glGetError())
    {
        hasError = true;
        
        std::cout << "[OpenGL Error](" << error << "): " <<
        function << " in " << file << ", line: " << line <<
        std::endl;
    }
    
    return !hasError;
}
