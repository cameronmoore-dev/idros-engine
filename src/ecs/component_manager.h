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

        template<typename T>
        bool has(Entity entity)
        {
            u64 key = std::type_index(typeid(T)).hash_code();
            ComponentSet<T> &set = *static_cast<ComponentSet<T>*>(m_pool.at(key));

            return (set.getDenseIndex(entity) != k_nullEntityID);
        }

        private:
            ComponentManager() = default;
    };

    #include "component_manager.inl"
}