#include "../shader.h"

#include <glad/gl.h>

#include <sstream>
#include <fstream>
#include <iostream>

namespace idrs
{
    Shader::Shader(const char* vertPath, const char* fragPath)
    {
        load(std::string(vertPath), std::string(fragPath));
    }

    void Shader::load(const std::string &vertPath, const std::string &fragPath)
    {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        std::string vertCode = vShaderStream.str();
        std::string fragCode = fShaderStream.str();

        u32 vertex = createVertexShader(vertCode.c_str());
        u32 fragment = createFragmentShader(fragCode.c_str());
        createShaderProgram(vertex, fragment);
    }
    
    void Shader::use()
    {
        glUseProgram(m_id);
    }

    void Shader::setBool(const std::string& name, bool value) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform1iEXT(m_id, loc, (int)value);
    }

    void Shader::setInt(const std::string& name, int value) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform1iEXT(m_id, loc, value);
    }

    void Shader::setFloat(const std::string& name, float value) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform1fEXT(m_id, loc, value);
    }

    void Shader::setVec2(const std::string& name, const Vec2f& value)
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform2fvEXT(m_id, loc, 1, &value.x);
    }

    void Shader::setVec2(const std::string& name, float x, float y) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform2fEXT(m_id, loc, x, y);
    }

    void Shader::setVec3(const std::string& name, const Vec3f& value) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform3fvEXT(m_id, loc, 1, &value.x);
    }

    void Shader::setVec3(const std::string& name, float x, float y, float z) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform3fEXT(m_id, loc, x, y, z);
    }

    void Shader::setVec4(const std::string& name, const Vec4f& value) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform4fvEXT(m_id, loc, 1, &value.x);
    }

    void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniform4fEXT(m_id, loc, x, y, z, w);
    }

    void Shader::setMat2(const std::string& name, const Mat2& mat) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniformMatrix2fvEXT(m_id, loc, 1, GL_TRUE, &mat[0][0]);
    }

    void Shader::setMat3(const std::string& name, const Mat3& mat) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniformMatrix3fvEXT(m_id, loc, 1, GL_TRUE, &mat[0][0]);
    }

    void Shader::setMat4(const std::string& name, const Mat4& mat) const
    {
        GLint loc = getUniformLocation(name);
        glProgramUniformMatrix4fvEXT(m_id, loc, 1, GL_TRUE, &mat[0][0]);
    }

    u32 Shader::createVertexShader(const char* shaderCode)
    {
        u32 vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &shaderCode, NULL);
        glCompileShader(vertex);

        int success;
        char infoLog[512];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
        	glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        	printf("Shader compile error:\n%s\n", infoLog);
        }

        return vertex;
    }

    u32 Shader::createFragmentShader(const char* shaderCode)
    {
        u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &shaderCode, NULL);
        glCompileShader(fragment);

        int success;
        char infoLog[512];
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
        	glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        	printf("Shader compile error:\n%s\n", infoLog);
        }

        return fragment;
    }

    void Shader::createShaderProgram(u32 vert, u32 frag)
    {
        m_id = glCreateProgram();
        glAttachShader(m_id, vert);
        glAttachShader(m_id, frag);
        glLinkProgram(m_id);

        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    const s32 Shader::getUniformLocation(const std::string &name) const
    {
        if (m_uniformLocations.find(name) != m_uniformLocations.end())
        {
            return m_uniformLocations[name];
        }

        GLint location = glGetUniformLocation(m_id, name.c_str());
        m_uniformLocations[name] = location;
        return location;
    }
}
