template<typename T>
inline void ComponentSet<T>::remove(Entity entity)
{
    u32 index = getDenseIndex(entity);
    if (index == k_nullEntityID)
    {
        return;
    }

    Entity back = entities.back();

    std::swap(dense[index], dense.back());
    std::swap(entities[index], entities.back());
    setDenseIndex(back, index);
    setDenseIndex(entity, k_nullEntityID);
    dense.pop_back();
    entities.pop_back();
}

template<class T>
inline void ComponentSet<T>::setDenseIndex(Entity entity, u32 index)
{
    u32 page = entity / k_pageSize;
    u32 pageIndex = entity % k_pageSize;

    if (page >= (u32)sparse.size())
    {
        sparse.resize(page + 1);
        sparse[page].fill(k_nullEntityID);
    }
    sparse[page][pageIndex] = index;
}

template<class T>
inline u32 ComponentSet<T>::getDenseIndex(Entity entity)
{
    u32 page = entity / k_pageSize;
    u32 pageIndex = entity % k_pageSize;

    if (page < (u32)sparse.size())
    {
        return sparse[page][pageIndex];
    }
    return k_nullEntityID;
}

template <class T>
inline size_t ComponentSet<T>::size()
{
    return entities.size();
}

template <class T>
inline std::vector<Entity> &ComponentSet<T>::entityList()
{
    return entities;
}