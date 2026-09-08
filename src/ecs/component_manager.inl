template<typename T>
inline T &ComponentManager::add(Entity entity)
{
    u64 key = std::type_index(typeid(T)).hash_code();

    if (m_pool.find(key) == m_pool.end())
    {
        m_pool.emplace(key, new ComponentSet<T>());
    }
    else if (!m_pool[key])
    {
        m_pool[key] = new ComponentSet<T>();
    }
    ComponentSet<T> &set = *static_cast<ComponentSet<T>*>(m_pool.at(key));

    set.setDenseIndex(entity, (u32)set.dense.size());
    u32 denseIndex = set.getDenseIndex(entity);

    T component = {};
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
inline std::vector<Entity> &ComponentManager::getEntities()
{
    u64 key = std::type_index(typeid(T)).hash_code();
    return m_pool.at(key)->entityList();
}

template <typename T>
inline bool ComponentManager::has(Entity entity)
{
    u64 key = std::type_index(typeid(T)).hash_code();

    /* NOTE: Even if a component is in the map, return false if it isn't initialized */
    ComponentSet<T> *set = static_cast<ComponentSet<T>*>(m_pool.at(key));
    return (set != nullptr)
        ? (set->getDenseIndex(entity) != k_nullEntityID)
        : false;
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
        if (s)
        {
            s->remove(entity);
        }
    }
}

inline void ComponentManager::clear()
{
    for (auto &[k, v] : m_pool)
    {
        delete v;
    }
    m_pool.clear();
}

template <typename... Components, typename Fn>
inline void ComponentManager::query(Fn fn)
{
    std::array<IComponentSet*, sizeof...(Components)> sets =
    {
        static_cast<IComponentSet*>(m_pool[std::type_index(typeid(Components)).hash_code()])...
    };

    /* NOTE: Searching for a null component set, if one is found then the query is aborted */
    auto it = std::find(sets.begin(), sets.end(), nullptr);
    if (it != sets.end())
    {
        return;
    }

    auto smallestSet = *std::min_element(sets.begin(), sets.end(), [](IComponentSet *a, IComponentSet *b)
    {
        return a->size() < b->size();
    });

    for (Entity e : smallestSet->entityList())
    {
        bool has_needed = (has<Components>(e) && ...);
        if (has_needed)
        {
            fn(e, get<Components>(e)...);
        }
    }
}
