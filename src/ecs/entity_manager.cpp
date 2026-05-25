#include "entity_manager.h"

#include "event_callback_handler.h"

#include <pal/idrs_time.h>
#include <cstdio>

namespace idrs
{
    EntityManager::EntityManager()
    {
        m_entities.reserve(k_maxEntities);
        m_activeIds.resize(k_maxEntities, false);

        EventCallbackHandler::subscribe<OnFrameStart>(std::bind(&EntityManager::addPending, this));
        EventCallbackHandler::subscribe<OnFrameEnd>(std::bind(&EntityManager::removePending, this));
    }

    Entity EntityManager::create()
    {
        Entity entity = getFirstFreeID();
        m_toAdd.emplace_back(entity);

        return entity;
    }

    void EntityManager::destroy(Entity entity)
    {
        m_toRemove.emplace_back(entity);
    }

    void EntityManager::clear()
    {
        m_entities.clear();
    }

    void EntityManager::addPending()
    {
        if (m_toAdd.empty())
        {
            return;
        }
        for (Entity pending : m_toAdd)
        {
            m_entities.emplace_back(pending);
        }

        m_toAdd.clear();
    }

    void EntityManager::removePending()
    {
        if (m_toRemove.empty())
        {
            return;
        }

        std::sort(m_toRemove.begin(), m_toRemove.end());
        m_toRemove.erase(std::unique(m_toRemove.begin(), m_toRemove.end()), m_toRemove.end());
        m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [this](const Entity e)
        {
            bool match = std::binary_search(m_toRemove.begin(), m_toRemove.end(), e);
            if (match) m_activeIds[e] = false;
            
            return match;
        }), m_entities.end());

        m_toRemove.clear();
    }

    Entity EntityManager::getFirstFreeID()
    {
        for (u32 i = 0; i < m_activeIds.size(); i++)
        {
            if (!m_activeIds.at(i))
            {
                m_activeIds[i] = true;
                return i;
            }
        }

        return k_nullEntityID;
    }
}