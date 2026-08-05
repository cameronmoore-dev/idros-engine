#pragma once

#include "entity_manager.h"
#include "component_manager.h"

namespace idrs
{
    class ECS
    {
    friend class Engine;
    public:
        static Entity create();
        static void destroy(Entity entity);
        static void destroyAll();

        template<typename T>
        static T &add(Entity entity);

        template<typename T>
        static T &get(Entity entity);

        template<typename T>
        static std::vector<Entity> &getEntities();

        template<typename T>
        static void remove(Entity entity);

        template<typename... Components, typename Fn>
        static void query(Fn fn);

        template<typename T>
        static bool has(Entity entity);

    private:
        EntityManager m_entityManager;
        ComponentManager m_componentManager;

    private:
        static ECS &get();
        ECS() = default;

        static void init();
    };

    #include "ecs.inl"
}
