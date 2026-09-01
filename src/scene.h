#pragma once

#include "core/window/window.h"
#include "core/graphics/renderer.h"

namespace idrs
{
	class Scene
	{
	public:
		Scene() = default;
		Scene(Window &window) {}
		virtual ~Scene() = default;
	
		virtual void start() = 0;
		virtual void processEvents(const Event &event) = 0;
		virtual void fixedUpdate() = 0;
		virtual void update() = 0;
		virtual void draw(Renderer &renderer) = 0;
		virtual void shutdown() = 0;
	};
}
