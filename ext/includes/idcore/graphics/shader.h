#pragma once

#include <string>
#include <unordered_map>

#include "typedefs.h"
#include "math.hpp"

namespace idrs 
{
    class Shader
    {
    public:
        Shader() = default;
        Shader(const char* vertPath, const char* fragPath);

        void load(const std::string &vertPath, const std::string &fragPath);

        void use();
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setBool(const std::string& name, bool value) const;
        void setVec2(const std::string& name, const Vec2f& value);
        void setVec2(const std::string& name, float x, float y) const;
        void setVec3(const std::string& name, const Vec3f& value) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setVec4(const std::string& name, const Vec4f& value) const;
        void setVec4(const std::string& name, float x, float y, float z, float w) const;
        void setMat2(const std::string& name, const Mat2& mat) const;
        void setMat3(const std::string& name, const Mat3& mat) const;
        void setMat4(const std::string& name, const Mat4& mat) const;

        inline u32 id() { return m_id; }

    private:
        u32 m_id = 0;
        mutable std::unordered_map<std::string, u32> m_uniformLocations;

    private:
        u32 createVertexShader(const char* shaderCode);
        u32 createFragmentShader(const char* shaderCode);
        void createShaderProgram(u32 vert, u32 frag);

        const s32 getUniformLocation(const std::string &name) const;
    };
}
