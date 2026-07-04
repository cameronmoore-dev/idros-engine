#pragma once

#include <unordered_map>

#include "scene.h"

namespace idrs
{
	class SceneManager
	{
	public:
		template<typename T>
		static void includeScene(Window &window, const std::string &name);
		static void loadScene(const std::string &name);

		static Scene &currentScene();
	
	private:
		Scene *m_scene = nullptr;
		std::unordered_map<u64, Scene*> m_sceneTable;

	private:
		u64 hash(const std::string &name);
	
	private:
		static SceneManager &get();
		SceneManager() = default;
		~SceneManager() = default;
	};

	#include "scene_manager.inl"
}
