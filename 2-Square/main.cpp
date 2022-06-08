#define GL_SILENCE_DEPRECATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Shaders.h"

void Cb(int code, const char* message)
{
    std::cout << "GL error Handled" << code << std::endl << message << std::endl;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwSetErrorCallback(Cb);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Divan genuine suff", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "NO WINDOW" << std::endl;
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    int status = gladLoadGL();
    std::cout << "GLad status: " << status << std::endl;
    
    // data to store on GPU
    float position[6] = {
      -0.5f, -0.5f,
       0.0f,  0.5f,
       0.5f, -0.5f
    };
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    

    // Get ID of memory address, where we will store data
    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    // Define how much space we need to have in a row
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);
    
    
    // Enable attribution
    glEnableVertexAttribArray(0);
    // Tell GPU what is inside buffer (specify layout of buffer): 0,
    // 2 floats inside vertex, 8 bytes until another vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    
    ShaderProgramSource shader = ParseShader("Shaders/Basic.shader");
    
    std::cout << shader.VertexSource << std::endl;
    std::cout << shader.FragmentSource << std::endl;
    
    unsigned int program = CreateShader(shader.VertexSource, shader.FragmentSource);
    glUseProgram(program);
    
    
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}

