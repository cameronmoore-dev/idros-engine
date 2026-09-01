#include "resource_manager.h"

#include <utility>

namespace idrs
{
    void ResourceManager::setLoadFlag(const LoadFlag flag)
    {
        m_flag = flag;
    }

    Shader &ResourceManager::loadShader(const std::filesystem::path &vpath, const std::filesystem::path &fpath)
    {
        std::shared_ptr<Shader> resource = std::make_shared<Shader>();
        resource->load(vpath.string(), fpath.string());

        return add<Shader>(vpath, resource);
    }

    template <typename T, typename... Args>
    T &ResourceManager::load(const std::filesystem::path &path, Args... args)
    {
        std::shared_ptr<T> resource = std::make_shared<T>();
        resource->load(path.string(), std::forward<Args>(args)...);

        return add<T>(path, resource);
    }

    template<typename T, typename... Args>
    void ResourceManager::loadDirectory(const std::filesystem::path &path, Args... args)
    {
        for (const auto &dir : std::filesystem::directory_iterator(path))
        {
            load<T>(dir.path(), args...);
        }
    }

    template<typename T>
    T &ResourceManager::getResource(const std::string &name)
    {
        u64 key = hash(name);

        auto it = m_sceneResources.find(key);
        if (it != m_sceneResources.end())
        {
            return *std::get<std::shared_ptr<T>>(it->second);
        }

        it = m_commonResources.find(key);
        if (it != m_commonResources.end())
        {
            return *std::get<std::shared_ptr<T>>(it->second);
        }
        else
        {
            printf("Couldn't find resource: %s\n", name.c_str());
            std::abort();
        }
    }

    template <typename T>
    T &ResourceManager::add(const std::filesystem::path &path, std::shared_ptr<T> resource)
    {
        u64 key = hash(getResourceName(path.string()));
        if (m_flag == LoadFlag::Common)
        {
            m_commonResources.emplace(key, resource);
            return *std::get<std::shared_ptr<T>>(m_commonResources.at(key));
        }

        m_sceneResources.emplace(key, resource);
        return *std::get<std::shared_ptr<T>>(m_sceneResources.at(key));
    }

    void ResourceManager::clearAll()
    {
        clearCommonResources();
        clearSceneResources();
    }

    void ResourceManager::clearCommonResources()
    {
        m_commonResources.clear();
    }

    void ResourceManager::clearSceneResources()
    {
        m_sceneResources.clear();
    }

    const u64 ResourceManager::hash(const std::string &name)
    {
        return std::hash<std::string>{}(name);
    }

    const std::string ResourceManager::getResourceName(const std::string &path)
    {
        size_t start = path.find_last_of("/\\") + 1;
        size_t end   = path.find_last_of(".");
        size_t len   = end - start;
        return path.substr(start, len);
    }

    template Texture &ResourceManager::load<Texture>(const std::filesystem::path&, bool, bool);
    template Texture &ResourceManager::load<Texture>(const std::filesystem::path&);
    template Image &ResourceManager::load<Image>(const std::filesystem::path&);
    template Music &ResourceManager::load<Music>(const std::filesystem::path&);
    template Sound &ResourceManager::load<Sound>(const std::filesystem::path&);
    template Font &ResourceManager::load<Font>(const std::filesystem::path&, s32);
    template Font &ResourceManager::load<Font>(const std::filesystem::path&);
    template SpriteAnimation &ResourceManager::load<SpriteAnimation>(const std::filesystem::path&);
    
    template void ResourceManager::loadDirectory<Texture>(const std::filesystem::path&, bool, bool);
    template void ResourceManager::loadDirectory<Texture>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Image>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Music>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Sound>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<Font>(const std::filesystem::path&, s32);
    template void ResourceManager::loadDirectory<Font>(const std::filesystem::path&);
    template void ResourceManager::loadDirectory<SpriteAnimation>(const std::filesystem::path&);

    template Texture &ResourceManager::getResource<Texture>(const std::string&);
    template Image &ResourceManager::getResource<Image>(const std::string&);
    template Shader &ResourceManager::getResource<Shader>(const std::string&);
    template Music &ResourceManager::getResource<Music>(const std::string&);
    template Sound &ResourceManager::getResource<Sound>(const std::string &);
    template Font &ResourceManager::getResource<Font>(const std::string &);
    template SpriteAnimation &ResourceManager::getResource<SpriteAnimation>(const std::string &);

    template Texture &ResourceManager::add<Texture>(const std::filesystem::path&, std::shared_ptr<Texture>);
    template Image &ResourceManager::add<Image>(const std::filesystem::path&, std::shared_ptr<Image>);
    template Shader &ResourceManager::add<Shader>(const std::filesystem::path&, std::shared_ptr<Shader>);
    template Music &ResourceManager::add<Music>(const std::filesystem::path&, std::shared_ptr<Music>);
    template Sound &ResourceManager::add<Sound>(const std::filesystem::path&, std::shared_ptr<Sound>);
    template Font &ResourceManager::add<Font>(const std::filesystem::path&, std::shared_ptr<Font>);
    template SpriteAnimation &ResourceManager::add<SpriteAnimation>(const std::filesystem::path&, std::shared_ptr<SpriteAnimation>);
}
