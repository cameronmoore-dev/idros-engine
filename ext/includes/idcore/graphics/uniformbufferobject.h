#pragma once

#include "shader.h"

namespace idrs
{
    class UniformBufferObject
    {
    public:
        UniformBufferObject(const std::string &name, u32 bytes, u32 bindingPoint);
        void init();
        void update(u32 offset, u32 bytes, void *value);
        void setBindingPoint(Shader &shader);

    private:
        std::string m_name;
        u32 m_index;
        u32 m_size;
        u32 m_bindingpoint;
    };
}
