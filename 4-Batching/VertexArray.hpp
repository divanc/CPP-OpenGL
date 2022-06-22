//
//  VertexArray.hpp
//  3-More
//
//  Created by Ivan on 10.06.2022.
//
#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int m_RendererID;
    
public:
    VertexArray();
    ~VertexArray();
    
    void Bind() const;
    void Unbind() const;
    
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    
};
