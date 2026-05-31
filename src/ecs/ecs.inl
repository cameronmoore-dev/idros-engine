template<typename T>
inline T &ECS::add(Entity entity)
{
    return get().m_componentManager.add<T>(entity);
}

template <typename T>
inline T &ECS::get(Entity entity)
{
    return get().m_componentManager.get<T>(entity);
}

template <typename T>
inline void ECS::remove(Entity entity)
{
    return get().m_componentManager.remove<T>(entity);
}

template<typename T>
inline std::vector<Entity> &ECS::view()
{
    return get().m_componentManager.view<T>();
}

template <typename... Components, typename Fn>
inline void ECS::_view(Fn fn)
{
    std::array<IComponentSet*, sizeof...(Components)> setlist =
    {
        static_cast<IComponentSet*>(get().m_componentManager.m_pool.at(std::type_index(typeid(Components)).hash_code()))...
    };

    auto smallestSet = *std::min_element(setlist.begin(), setlist.end(), [](IComponentSet *a, IComponentSet *b)
    {
        return a->size() < b->size();
    });

    for (Entity e : smallestSet->entityList())
    {
        bool has_needed = (get().m_componentManager.has<Components>(e) && ...);
        if (has_needed)
        {
            fn(get<Components>(e)...);
        }
    }
}