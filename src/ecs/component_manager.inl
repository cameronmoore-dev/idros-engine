template<typename T>
inline T &ComponentManager::add(Entity entity)
{
    u64 key = std::type_index(typeid(T)).hash_code();
    if (m_pool.find(key) == m_pool.end())
    {
        m_pool.emplace(key, new ComponentSet<T>());
    }
    ComponentSet<T> &set = *static_cast<ComponentSet<T>*>(m_pool.at(key));

    set.setDenseIndex(entity, (u32)set.dense.size());
    u32 denseIndex = set.getDenseIndex(entity);

    T component = T{};
    set.dense.emplace_back(component);
    set.entities.emplace_back(entity);
    return set.dense[denseIndex];
}

template<typename T>
inline T &ComponentManager::get(Entity entity)
{
    u64 key = std::type_index(typeid(T)).hash_code();
    ComponentSet<T> &set = *static_cast<ComponentSet<T>*>(m_pool.at(key));

    u32 index = set.getDenseIndex(entity);
    return set.dense[index];
}

template<typename T>
inline void ComponentManager::remove(Entity entity)
{
    u64 key = std::type_index(typeid(T)).hash_code();
    ComponentSet<T> &set = *static_cast<ComponentSet<T>*>(m_pool.at(key));

    set.remove(entity);
}

inline void ComponentManager::removeAll(Entity entity)
{
    for (const auto &[hash, set] : m_pool)
    {
        auto *s = dynamic_cast<IComponentSet*>(set);
        s->remove(entity);
    }
}

template <typename T>
inline std::vector<Entity> &ComponentManager::view()
{
    u64 key = std::type_index(typeid(T)).hash_code();
    ComponentSet<T> &set = *static_cast<ComponentSet<T>*>(m_pool.at(key));

    return set.entities;
}