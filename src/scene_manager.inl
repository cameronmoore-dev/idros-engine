template<typename T>
inline void SceneManager::includeScene(Window &window, const std::string &name)
{
    u64 key = hash(name);
    m_sceneTable.emplace(key, new T(window));
}