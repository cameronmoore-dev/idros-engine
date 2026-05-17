#pragma once

namespace idrs
{
	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;
	
		virtual void start() = 0;
		virtual void processEvents() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void shutdown() = 0;
	
		virtual void resume() = 0;
		virtual void pause() = 0;
	};
}