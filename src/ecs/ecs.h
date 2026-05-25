#pragma once

#include "entity_manager.h"

namespace idrs
{
    class ECS
    {
    friend class Engine;

    public:
        static Entity create();
        static void destroy(Entity entity);
        static void destroyAll();

        // template<typename T, typename... Args>
        // static T &add(Entity entity, Args&&... args);

        // template <typename T>
        // static T &get(Entity entity);

        // template<typename T>
        // static void remove(Entity entity);

        // template<typename T>
        // static bool has(Entity entity);

    private:
        EntityManager m_entityManager;
        // ComponentManager

    private:
        static ECS &get();
        ECS() = default;
    };

    #include "ecs.inl"
}