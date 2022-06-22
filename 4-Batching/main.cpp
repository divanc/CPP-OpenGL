#pragma mark - Overview
/*
 In this lesson we are:
 
 * 1. Abstracting GL logic to classes. We deduce:
 - *  VertexBuffer
 - *  IndexBuffer
 - *  VertexArray
 - *  VertexBufferLayout
 - *  Shader
 - *  Renderer
 -    All of these replace all GL calls
 
 * 2. Texturing
 -    Loading an image to our GPU, exploding it to
 -    a number of pixels. Setting a valid alpha behaviour
 
 * 3. Introduce MVP as a camera matrix and recalculate every frame
 -    inside a vertex shader
 
 * 4. Implement imGui and feed translation matrices into it
 */

#pragma mark - Precompilation

#define GL_SILENCE_DEPRECATION
#define SIZE 50.0f
#define VSYNC 1

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "vendor/imgui/imgui_impl_glfw.h"

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
    std::cout << "GLFW is fine!" << std::endl;
    
    setOpenGLEnviroment();
    
#pragma mark - Create Window
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Divan genuine suff", NULL, NULL);
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
        -SIZE/2, -SIZE/2, 0.0f, 0.0f, // 0
         SIZE/2, -SIZE/2, 1.0f, 0.0f, // 1
         SIZE/2,  SIZE/2, 1.0f, 1.0f, // 2
        -SIZE/2,  SIZE/2, 0.0f, 1.0f // 3
    };
    
    unsigned int indexBuffer[] = {
        0, 1, 2,
        2, 3, 0,
    };
    
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    VertexArray va;
    
    VertexBuffer vb(vertexBuffer, 4 * 4 * sizeof(float));
    IndexBuffer ib(indexBuffer, 6);
    
    glm::mat4 proj = glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    
    va.AddBuffer(vb, layout);
    
    
    Shader shader("Shaders/Basic.shader");
    shader.Bind();
    
    
#pragma mark - Passing color from CPU via Uniform
    float red = 0.41f;
    float green = 0.84f;
    float blue = 0.88f;
    float alpha = 1.0f;
    
    shader.SetUniform4f("u_Color", red, green, blue, alpha);
    
#pragma mark - Draw loop
    
    Texture texture("res/textures/gopher.png");
    texture.Bind();
    
    shader.SetUniform1i("u_Texture", 0);
    
    // Drop
    shader.Unbind();
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;
    
    float modifier = 1.1f;
    
    Renderer renderer;
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    glm::vec3 translation1(50, 0, 0);
    glm::vec3 translation2(90, 0, 0);
    
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        shader.Bind();
        shader.SetUniform4f("u_Color", red, green, blue, alpha);
        
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation1);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            
            renderer.Draw(va, ib, shader);
        }
        
        {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation2);
        glm::mat4 mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);
        
        renderer.Draw(va, ib, shader);
        }
        
        if (modifier > 0.0f) // ascending
        {
            if (green >= 1.0f) modifier = -0.05f;
            else green *= modifier;
        }
        else // descending
        {
            if (green <= 0.0f)
            {
                green = 0.8f;
                modifier = 1.2f;
            }
            else green += modifier;
        }
        
        ImGui::Begin("Hello, world!");
        ImGui::SliderFloat3("Translation 1", &translation1.x, 0.0f, 100.0f);
        ImGui::SliderFloat3("Translation 2", &translation2.x, 0.0f, 100.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    GLCall(glfwTerminate());
    
    return 0;
}

