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