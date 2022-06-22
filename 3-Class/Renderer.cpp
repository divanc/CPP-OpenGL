//
//  Renderer.cpp
//  3-More
//
//  Created by Ivan on 10.06.2022.
//

#include <iostream>

#include "Renderer.h"


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
    {
        
    }
}

bool GLLogCall(const char* function, const char* file, int line)
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

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
   
}
