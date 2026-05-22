template<typename T>
inline void SceneManager::includeScene(Window &window, const std::string &name)
{
    SceneManager &instance = get();

    u64 key = instance.hash(name);
    instance.m_sceneTable.emplace(key, new T(window));
}