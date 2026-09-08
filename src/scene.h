#pragma once

#include "core/window/event.h"

namespace idrs
{
	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;
	
		virtual void start() = 0;
		virtual void processEvents(const Event &event) = 0;
		virtual void fixedUpdate() = 0;
		virtual void update() = 0;
		virtual void draw() = 0;
		virtual void shutdown() = 0;
	};
}
