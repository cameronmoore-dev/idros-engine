#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <utility>
#include <filesystem>

#include <pal/idros_pal.h>

namespace idrs
{
    using ResourceVar = std::variant<std::shared_ptr<Texture>, 
                                     std::shared_ptr<Image>, 
                                     std::shared_ptr<Music>, 
                                     std::shared_ptr<Sound>, 
                                     std::shared_ptr<Font>>;

    class ResourceManager
    {
    public:
        /* 
         * @brief Loads resource to be used exclusively within the scene 
         * @param path: File path to resource
         */
        template <typename T, typename... Args>
        static T &loadInScene(const std::filesystem::path &path, Args... args);

        template<typename T, typename... Args>
        static T &load(const std::filesystem::path &path, Args... args);

        template<typename T, typename... Args>
        static void loadDirectoryInScene(const std::filesystem::path &path, Args... args);

        template <typename T, typename... Args>
        static void loadDirectory(const std::filesystem::path &path, Args... args);

        template<typename T>
        static T &getResource(const std::filesystem::path &name);

        static void clearAll();
        static void clearCommonResources();
        static void clearSceneResources();

    private:
        std::unordered_map<u64, ResourceVar> m_commonResources;
        std::unordered_map<u64, ResourceVar> m_sceneResources;

    private:
        const u64 hash(const std::string &resName);
        const std::string getResourceName(const std::string &path);

    private:
        static ResourceManager &get();
        ResourceManager() = default;
        ~ResourceManager() = default;
    };
}