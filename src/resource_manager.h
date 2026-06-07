#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <utility>
#include <filesystem>

#include <idlai/graphics/texture.h>
#include <idlai/graphics/image.h>
#include <idlai/graphics/shader.h>
#include <idlai/graphics/font.h>
#include <idlai/audio/sound.h>
#include <idlai/audio/music.h>

#include "sprite_animation.h"

namespace idrs
{
    class ResourceManager
    {
    using ResourceVar = std::variant<
        std::shared_ptr<Texture>, 
        std::shared_ptr<Image>, 
        std::shared_ptr<SpriteAnimation>,
        std::shared_ptr<Shader>,
        std::shared_ptr<Music>, 
        std::shared_ptr<Sound>, 
        std::shared_ptr<Font>>;

    public:
        enum class LoadFlag
        {
            Common,
            Scene
        };

    public:
        static void setLoadFlag(const LoadFlag flag);

        template<typename T, typename... Args>
        static T &load(const std::filesystem::path &path, Args... args);

        template<typename T, typename... Args>
        static void loadDirectory(const std::filesystem::path &path, Args... args);

        static Shader &loadShader(const std::filesystem::path &vpath, const std::filesystem::path &fpath);

        template<typename T>
        static T &getResource(const std::string &name);

        static void clearAll();
        static void clearCommonResources();
        static void clearSceneResources();

    private:
        std::unordered_map<u64, ResourceVar> m_commonResources;
        std::unordered_map<u64, ResourceVar> m_sceneResources;

        LoadFlag m_flag = LoadFlag::Common;

    private:
        const u64 hash(const std::string &resName);
        const std::string getResourceName(const std::string &path);

        template <typename T>
        static T &add(const std::filesystem::path &path, std::shared_ptr<T> resource);

    private:
        static ResourceManager &get();
        ResourceManager() = default;
        ~ResourceManager() = default;
    };
}