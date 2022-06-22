//
//  VertexBuffer.h
//  3-More
//
//  Created by Ivan on 10.06.2022.
//
#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererID;
    
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};
