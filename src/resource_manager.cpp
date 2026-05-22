#include "resource_manager.h"

namespace idrs
{
    ResourceManager &ResourceManager::get()
    {
        static ResourceManager instance;
        return instance;
    }

    template<typename T, typename... Args>
    T &ResourceManager::loadInScene(const std::filesystem::path &path, Args... args)
    {
        ResourceManager &instance = get();

        std::shared_ptr<T> resource = std::make_shared<T>();
        resource->load(path.string(), std::forward<Args>(args)...);

        u64 key = instance.hash(instance.getResourceName(path.string()));

        instance.m_sceneResources.emplace(key, resource);
        return *std::get<std::shared_ptr<T>>(instance.m_sceneResources.at(key));
    }

    template<typename T, typename... Args>
    T &ResourceManager::load(const std::filesystem::path &path, Args... args)
    {
        ResourceManager &instance = get();

        std::shared_ptr<T> resource = std::make_shared<T>();
        resource->load(path.string(), std::forward<Args>(args)...);

        u64 key = instance.hash(instance.getResourceName(path.string()));

        instance.m_commonResources.emplace(key, resource);
        return *std::get<std::shared_ptr<T>>(instance.m_commonResources.at(key));
    }

    template<typename T, typename... Args>
    void ResourceManager::loadDirectoryInScene(const std::filesystem::path &path, Args... args)
    {
        IDRS_ASSERT(std::filesystem::exists(path), "Path doesn't lead to a valid directory: " + path.string());
        for (const auto& dir : std::filesystem::directory_iterator(path))
        {
            loadInScene<T>(dir.path(), args...);
        }
    }

    template<typename T, typename... Args>
    void ResourceManager::loadDirectory(const std::filesystem::path &path, Args... args)
    {
        IDRS_ASSERT(std::filesystem::exists(path), "Path doesn't lead to a valid directory: " + path.string());
        for (const auto &dir : std::filesystem::directory_iterator(path))
        {
            load<T>(dir.path(), args...);
        }
    }

    template<typename T>
    T &ResourceManager::getResource(const std::filesystem::path &name)
    {
        ResourceManager &instance = get();
        u64 key = instance.hash(name.string());

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
            printf("Couldn't find resource: %s\n", name.string().c_str());
            std::abort();
        }
    }

    void ResourceManager::clearAll()
    {
        clearCommonResources();
        clearSceneResources();
    }

    void ResourceManager::clearCommonResources()
    {
        get().m_commonResources.clear();
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

    template Texture &ResourceManager::loadInScene<Texture>(const std::filesystem::path&, bool, bool);
    template Texture &ResourceManager::loadInScene<Texture>(const std::filesystem::path&);
    template Image &ResourceManager::loadInScene<Image>(const std::filesystem::path&);
    template Music &ResourceManager::loadInScene<Music>(const std::filesystem::path&);
    template Sound &ResourceManager::loadInScene<Sound>(const std::filesystem::path&);
    template Font &ResourceManager::loadInScene<Font>(const std::filesystem::path&, s32);
    template Font &ResourceManager::loadInScene<Font>(const std::filesystem::path&);

    template Texture &ResourceManager::load<Texture>(const std::filesystem::path &, bool, bool);
    template Texture &ResourceManager::load<Texture>(const std::filesystem::path &);
    template Image &ResourceManager::load<Image>(const std::filesystem::path &);
    template Music &ResourceManager::load<Music>(const std::filesystem::path &);
    template Sound &ResourceManager::load<Sound>(const std::filesystem::path &);
    template Font &ResourceManager::load<Font>(const std::filesystem::path &, s32);
    template Font &ResourceManager::load<Font>(const std::filesystem::path &);

    template void ResourceManager::loadDirectoryInScene<Texture>(const std::filesystem::path&, bool, bool);
    template void ResourceManager::loadDirectoryInScene<Texture>(const std::filesystem::path&);
    template void ResourceManager::loadDirectoryInScene<Image>(const std::filesystem::path&);
    template void ResourceManager::loadDirectoryInScene<Music>(const std::filesystem::path&);
    template void ResourceManager::loadDirectoryInScene<Sound>(const std::filesystem::path&);
    template void ResourceManager::loadDirectoryInScene<Font>(const std::filesystem::path&, s32);
    template void ResourceManager::loadDirectoryInScene<Font>(const std::filesystem::path&);

    template void ResourceManager::loadDirectory<Texture>(const std::filesystem::path&, bool, bool);
    template void ResourceManager::loadDirectory<Texture>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Image>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Music>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Sound>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Font>(const std::filesystem::path&, s32);
    template void ResourceManager::loadDirectory<Font>(const std::filesystem::path&);

    template Texture &ResourceManager::getResource<Texture>(const std::filesystem::path&);
    template Image &ResourceManager::getResource<Image>(const std::filesystem::path&);
    template Music &ResourceManager::getResource<Music>(const std::filesystem::path&);
    template Sound &ResourceManager::getResource<Sound>(const std::filesystem::path&);
    template Font &ResourceManager::getResource<Font>(const std::filesystem::path&);
}
