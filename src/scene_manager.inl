template<typename T>
void SceneManager::addScene(Window &window)
{
    SceneManager &instance = get();
    if (!instance.m_scenes.empty())
    {
        instance.m_scenes.pop();
    }

    instance.m_scenes.push(std::make_unique<T>(window));
    instance.m_scenes.top()->start();
}