#pragma once

#include <array>
#include <vector>

#include "entity.h"

namespace idrs
{
    class IComponentSet
    {
    public:
        ~IComponentSet() = default;
        virtual void remove(Entity entity) = 0;
    };

    constexpr inline u32 k_pageSize = 1024;
    using Page = std::array<u32, k_pageSize>;

    template<class T>
    class ComponentSet : public IComponentSet
    {
    public:
        std::vector<Page> sparse;
        std::vector<T> dense;
        std::vector<Entity> entities;

    public:
        void remove(Entity entity) override;

        void setDenseIndex(Entity entity, u32 index);
        u32 getDenseIndex(Entity entity);
    };

    #include "component_set.inl"
}