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
		static void addScene()
		{
			SceneManager &instance = get();
			if (!instance.m_scenes.empty())
				instance.m_scenes.pop();
	
			instance.m_scenes.push(std::make_unique<T>());
			instance.m_scenes.top()->start();
		}
	
		static std::unique_ptr<Scene> &getActiveScene()
		{
			return get().m_scenes.top();
		}
	
	private:
		std::stack<std::unique_ptr<Scene>> m_scenes;
	
	private:
		static SceneManager &get()
		{
			static SceneManager instance;
			return instance;
		}
	
		SceneManager() = default;
		~SceneManager() = default;
	};
}