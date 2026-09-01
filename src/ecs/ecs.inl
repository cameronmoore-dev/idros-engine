template<typename T>
inline T &ECS::add(Entity entity)
{
    return m_componentManager.add<T>(entity);
}

template <typename T>
inline T &ECS::get(Entity entity)
{
    return m_componentManager.get<T>(entity);
}

template<typename T>
inline std::vector<Entity> &ECS::getEntities()
{
    return m_componentManager.getEntities<T>();
}

template<typename T>
inline void ECS::remove(Entity entity)
{
    return m_componentManager.remove<T>(entity);
}

template<typename T>
inline bool ECS::has(Entity entity)
{
    return m_componentManager.has<T>(entity);
}

template<typename... Components, typename Fn>
inline void ECS::query(Fn fn)
{
    m_componentManager.query<Components...>(std::forward<Fn>(fn));
}
