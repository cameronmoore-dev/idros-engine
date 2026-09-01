#pragma once

#include <unordered_map>

#include "scene.h"

namespace idrs
{
	class SceneManager
	{
	public:
		SceneManager();

		template<typename T>
		void includeScene(Window &window, const std::string &name);
		void loadScene(const std::string &name);

		Scene &currentScene();
	
	private:
		Scene *m_scene;
		std::unordered_map<u64, Scene*> m_sceneTable;

	private:
		u64 hash(const std::string &name);
	};

	#include "scene_manager.inl"
}
