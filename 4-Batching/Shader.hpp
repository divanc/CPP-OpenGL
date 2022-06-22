#pragma once

#include <string>
#include <unordered_map>

#include "vendor/glm/glm.hpp"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader
{
private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    void SetUniform1i(const std::string& name, int value) const;
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void SetUniformMat4f(const std::string& name, const glm::mat4& value);
    
private:
    int GetUniformLocation(const std::string& name) const;
    unsigned int Create(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int Compile(unsigned int type, const std::string& source);
    ShaderProgramSource Parse(const std::string& filepath);
};
