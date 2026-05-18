#pragma once

#include <memory>
#include <stack>

#include "scene.h"

namespace idrs
{
	class SceneManager
	{
	public:
		template<typename T>
		static void addScene(Window &window);
		static void removeScene();

		static const std::unique_ptr<Scene> &getActiveScene();
	
	private:
		std::stack<std::unique_ptr<Scene>> m_scenes;
	
	private:
		static SceneManager &get();
	
		SceneManager() = default;
		~SceneManager() = default;
	};

	#include "scene_manager.inl"
}