#pragma mark - Overview
/*
 In this lesson we are:
 
 * 1. Drawing a rectangle made out of 2 triangles.
 -    As rect has 4 vertexes, while 2 triangles have 6
 -    we use IBO to reuse vertexes, which are already store in GPU
 -
 * 2. Define our shaders in a separate file and manually parse it
 -
 * 3. Create an debug error handler and apply it to every function
 -    Because OpenGL<4.3 is that way :(
 -
 * 4. Pass a color into a shader from CPU using uniforms,
 -    Then start a party!
 -
 * 5. Limit frame rate to not party too hard
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
#include "Errors.h"

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
    
    
    // Doesnt work without this
    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));
    
#pragma mark - Vertex Buffer init
    // data to store on GPU
    float vertexBuffer[] = {
        -SIZE, -SIZE, // 0
        SIZE, -SIZE, // 1
        SIZE,  SIZE, // 2
        -SIZE,  SIZE, // 3
    };
    
    // Get ID of memory address, where we will store vertex data
    unsigned int vbo;
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertexBuffer, GL_STATIC_DRAW));
    
    
#pragma mark - Index Buffer init
    // reuse vertexes with provided order
    unsigned int indexBuffer[] = {
        0, 1, 2,
        2, 3, 0,
    };
    
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexBuffer, GL_STATIC_DRAW));
    
    // Enable attribution
    GLCall(glEnableVertexAttribArray(0));
    // Tell GPU what is inside buffer (specify layout of buffer): 0,
    // 2 floats inside vertex, 8 bytes until another vertex
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    
    
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
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
    
    float modifier = 1.1f;
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        GLCall(glUniform4f(location, red, green, blue, alpha));
        
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

