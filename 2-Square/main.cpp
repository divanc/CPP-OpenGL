#pragma mark - Precompilation
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
        glfwTerminate();
        std::cout << "NO WINDOW" << std::endl;
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    int status = gladLoadGL();
    std::cout << "GLad status: " << status << std::endl;
    
    
    // Doesnt work without this
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
#pragma mark - Vertex Buffer init
    // data to store on GPU
    float vertexBuffer[] = {
        -0.5f, -0.5f, // 0
        0.5f, -0.5f, // 1
        0.5f,  0.5f, // 2
        -0.5f,  0.5f, // 3
    };
    
    // Get ID of memory address, where we will store vertex data
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertexBuffer, GL_STATIC_DRAW);
    
    
#pragma mark - Index Buffer init
    // reuse vertexes with provided order
    unsigned int indexBuffer[] = {
        0, 1, 2,
        2, 3, 0,
    };
    
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW);
    
    // Enable attribution
    glEnableVertexAttribArray(0);
    // Tell GPU what is inside buffer (specify layout of buffer): 0,
    // 2 floats inside vertex, 8 bytes until another vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    
    
#pragma mark - Shader Loading
    ShaderProgramSource shader = ParseShader("Shaders/Basic.shader");
    
    
    unsigned int program = CreateShader(shader.VertexSource, shader.FragmentSource);
    glUseProgram(program);
    
#pragma mark - Draw loop
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glDeleteProgram(program);
    glfwTerminate();
    
    return 0;
}

