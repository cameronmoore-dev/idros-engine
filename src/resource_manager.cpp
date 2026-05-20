#include "resource_manager.h"

namespace idrs
{
    template<typename T, typename... Args>
    T &ResourceManager::loadInScene(const std::string& path, Args... args)
    {
        ResourceManager &instance = get();

        std::shared_ptr<T> resource = std::make_shared<T>();
        resource->load(path, std::forward<Args>(args)...);

        u64 key = instance.hash(instance.getResourceName(path));

        instance.m_sceneResources.emplace(key, resource);
        return *std::get<std::shared_ptr<T>>(instance.m_sceneResources.at(key));
    }
    
    template<typename T, typename... Args>
    T &ResourceManager::load(const std::string& path, Args... args)
    {
        ResourceManager &instance = get();

        std::shared_ptr<T> resource = std::make_shared<T>();
        resource->load(path, std::forward<Args>(args)...);

        u64 key = instance.hash(instance.getResourceName(path));

        instance.m_commonResources.emplace(key, resource);
        return *std::get<std::shared_ptr<T>>(instance.m_commonResources.at(key));
    }

    template<typename T>
    T &ResourceManager::getResource(const std::string& name)
    {
        ResourceManager &instance = get();
        u64 key = instance.hash(name);

        auto it = instance.m_sceneResources.find(key);
        if (it != instance.m_sceneResources.end())
        {
            return *std::get<std::shared_ptr<T>>(it->second);
        }

        it = instance.m_commonResources.find(key);
        if (it != instance.m_commonResources.end())
        {
            return *std::get<std::shared_ptr<T>>(it->second);
        }
        else
        {
            printf("Couldn't find resource: %s\n", name.c_str());
            std::abort();
        }
    }

    void ResourceManager::clearSceneResources()
    {
        get().m_sceneResources.clear();
    }

    const u64 ResourceManager::hash(const std::string &name)
    {
        return std::hash<std::string>{}(name);
    }

    const std::string ResourceManager::getResourceName(const std::string &path)
    {
        size_t start = path.find_last_of("/") + 1;
        size_t end   = path.find_last_of(".");
        size_t len = end - start;
        return path.substr(start, len);
    }

    template Texture &ResourceManager::loadInScene<Texture>(const std::string&, bool, bool);
    template Texture &ResourceManager::loadInScene<Texture>(const std::string&);
    template Image &ResourceManager::loadInScene<Image>(const std::string&);
    template Music &ResourceManager::loadInScene<Music>(const std::string&);
    template Sound &ResourceManager::loadInScene<Sound>(const std::string&);
    template Font &ResourceManager::loadInScene<Font>(const std::string&, s32);
    template Font &ResourceManager::loadInScene<Font>(const std::string&);

    template Texture &ResourceManager::load<Texture>(const std::string&, bool, bool);
    template Texture &ResourceManager::load<Texture>(const std::string&);
    template Image &ResourceManager::load<Image>(const std::string&);
    template Music &ResourceManager::load<Music>(const std::string&);
    template Sound &ResourceManager::load<Sound>(const std::string&);
    template Font &ResourceManager::load<Font>(const std::string&, s32);
    template Font &ResourceManager::load<Font>(const std::string&);

    template Texture &ResourceManager::getResource<Texture>(const std::string&);
    template Image &ResourceManager::getResource<Image>(const std::string&);
    template Music &ResourceManager::getResource<Music>(const std::string&);
    template Sound &ResourceManager::getResource<Sound>(const std::string&);
    template Font &ResourceManager::getResource<Font>(const std::string&);
}
