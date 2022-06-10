#pragma mark - Overview
/*
 In this lesson we are:
 
 */

#pragma mark - Precompilation

#define GL_SILENCE_DEPRECATION
#define SIZE 0.96f
#define VSYNC 1

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shaders.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

void Cb(int code, const char* message)
{
    std::cout << "GL error Handled" << code << std::endl << message << std::endl;
}


#pragma mark - OpenGL hints
#pragma region GLFWHints {
void setOpenGLEnviroment()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwSetErrorCallback(Cb);
}
#pragma endregion GLFWHints }

#pragma mark - Main
int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    setOpenGLEnviroment();
    
#pragma mark - Create Window
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Divan genuine suff", NULL, NULL);
    if (!window)
    {
        GLCall(glfwTerminate());
        std::cout << "NO WINDOW" << std::endl;
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    // Vertical sync. 1 Limits frame rate to monitor hz
    glfwSwapInterval(VSYNC);
    
    int status = gladLoadGL();
    std::cout << "GLad status: " << status << std::endl;
    
    
    float vertexBuffer[] = {
        -SIZE, -SIZE, // 0
        SIZE, -SIZE, // 1
        SIZE,  SIZE, // 2
        -SIZE,  SIZE, // 3
    };
    
    unsigned int indexBuffer[] = {
        0, 1, 2,
        2, 3, 0,
    };
    
    VertexArray va;
    VertexBuffer vb(vertexBuffer, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
    
    IndexBuffer ib(indexBuffer, 6);
    
#pragma mark - Shader Loading
    GLCall(ShaderProgramSource source = ParseShader("Shaders/Basic.shader"));
    
    
    GLCall(unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource));
    GLCall(glUseProgram(shader));
    
    
#pragma mark - Passing color from CPU via Uniform
    float red = 0.2;
    float green = 0.22;
    float blue = 0.998;
    float alpha = 1.0;
    
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, red, green, blue, alpha));
    
#pragma mark - Draw loop
    
    
    // Drop
    GLCall(glUseProgram(0));
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
    
    float modifier = 1.1f;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, red, green, blue, alpha));
        
        va.Bind();
        
        
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        if (modifier > 0.0f) // ascending
        {
            if (red >= 1.0f) modifier = -0.05f;
            else red *= modifier;
        }
        else // descending
        {
            if (red <= 0.0f)
            {
                red = 0.8f;
                modifier = 1.2f;
            }
            else red += modifier;
        }
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));
        
        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    
    GLCall(glDeleteProgram(shader));
    GLCall(glfwTerminate());
    
    return 0;
}

