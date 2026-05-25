#pragma once

#include <typeindex>
#include <unordered_map>

#include "entity.h"
#include "component_set.h"

namespace idrs
{
    class ComponentManager
    {
    friend class ECS;
    private:
        std::unordered_map<u64, IComponentSet*> m_pool;

    private:
        template<typename T>
        T &add(Entity entity);

        template <typename T>
        T &get(Entity entity);

        template<typename T>
        void remove(Entity entity);

        void removeAll(Entity entity);

        template<typename T>
        std::vector<Entity> &view();

        /* 
         * NOTE: is there any practical use for a 'has' function?
         *       since the user will have an array of entities
         *       which must have a given component returned when querying for systems
         */
        // template<typename T>
        // bool has(Entity entity)
        // {
        // }

    private:
        ComponentManager() = default;
    };

    #include "component_manager.inl"
}