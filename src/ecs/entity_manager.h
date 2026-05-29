#pragma once

#include <vector>
#include <algorithm>

#include "entity.h"

namespace idrs
{
    class EntityManager
    {
    friend class ECS;

    private:
        std::vector<Entity> m_entities;
        std::vector<Entity> m_toAdd;
        std::vector<Entity> m_toRemove;
        std::vector<bool>   m_activeIds;

        const u32 k_maxEntities = 100'000;

    private:
        EntityManager();

        Entity create();
        void destroy(Entity entity);
        void clear();
        void addPending();
        void removePending();

        Entity getFirstFreeID();
    };
}