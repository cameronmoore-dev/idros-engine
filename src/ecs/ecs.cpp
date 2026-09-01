#include "ecs.h"

namespace idrs
{
    Entity ECS::create()
    {
        return m_entityManager.create();
    }

    void ECS::destroy(Entity entity)
    {
        m_entityManager.destroy(entity);
        m_componentManager.removeAll(entity);
    }

    void ECS::destroyAll()
    {
        m_entityManager.clear();
        m_componentManager.clear();
    }
}
