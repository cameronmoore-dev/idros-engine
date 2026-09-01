#pragma once

#include <memory>
#include <unordered_map>
#include <variant>
#include <filesystem>

#include "core/graphics/texture.h"
#include "core/graphics/image.h"
#include "core/graphics/shader.h"
#include "core/graphics/font.h"
#include "core/audio/sound.h"
#include "core/audio/music.h"

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
        ResourceManager() = default;

        void setLoadFlag(const LoadFlag flag);

        template<typename T, typename... Args>
        T &load(const std::filesystem::path &path, Args... args);

        template<typename T, typename... Args>
        void loadDirectory(const std::filesystem::path &path, Args... args);

        Shader &loadShader(const std::filesystem::path &vpath, const std::filesystem::path &fpath);

        template<typename T>
        T &getResource(const std::string &name);

        void clearAll();
        void clearCommonResources();
        void clearSceneResources();

    private:
        std::unordered_map<u64, ResourceVar> m_commonResources;
        std::unordered_map<u64, ResourceVar> m_sceneResources;

        LoadFlag m_flag = LoadFlag::Common;

    private:
        const u64 hash(const std::string &resName);
        const std::string getResourceName(const std::string &path);

        template <typename T>
        T &add(const std::filesystem::path &path, std::shared_ptr<T> resource);
    };
}
