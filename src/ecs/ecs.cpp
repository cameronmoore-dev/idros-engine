#include "ecs.h"

namespace idrs
{
    ECS &ECS::get()
    {
        static ECS instance;
        return instance;
    }

    Entity ECS::create()
    {
        return get().m_entityManager.create();
    }

    void ECS::destroy(Entity entity)
    {
        get().m_entityManager.destroy(entity);
    }
    
    void ECS::destroyAll()
    {
        get().m_entityManager.clear();
    }
}