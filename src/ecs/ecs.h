#pragma once

#include "entity_manager.h"
#include "component_manager.h"

namespace idrs
{
    class ECS
    {
    friend class Engine;
    public:
        ECS() = default;

        Entity create();
        void destroy(Entity entity);
        void destroyAll();

        template<typename T>
        T &add(Entity entity);

        template<typename T>
        T &get(Entity entity);

        template<typename T>
        std::vector<Entity> &getEntities();

        template<typename T>
        void remove(Entity entity);

        template<typename... Components, typename Fn>
        void query(Fn fn);

        template<typename T>
        bool has(Entity entity);

    private:
        EntityManager m_entityManager;
        ComponentManager m_componentManager;
    };

    #include "ecs.inl"
}
