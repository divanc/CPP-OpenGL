#define GL_SILENCE_DEPRECATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "lessons.h"

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
        (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
        " shader!" << std::endl;
        
        std::cout << message << std::endl;
        
        glDeleteShader(id);
        
        return 0;
    }
    
    return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

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
    // Tell GPU what is inside buffer (specify layout of buffer): 0, 2 floats inside vertex, 8 bytes until another vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    std::string vertexShader = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}
)glsl";
    
    std::string fragmentShader = R"glsl(
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.83, 0.1, 0.56, 0.7542);
}
)glsl";
    
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    
    
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
    
    glfwTerminate();
    return 0;
}

