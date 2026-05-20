#pragma once

#include "scene.h"

namespace idrs
{
	class SceneManager
	{
	public:
		template<typename T>
		static void loadScene(Window &window);
		static Scene &currentScene();
	
	private:
		Scene *m_scene = nullptr;
	
	private:
		static SceneManager &get();
		SceneManager() = default;
		~SceneManager() = default;
	};

	#include "scene_manager.inl"
}