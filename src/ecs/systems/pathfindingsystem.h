#pragma once

#include "ecs/components.h"

namespace idrs
{
    class PathfindingSystem
    {
    public:
        static void update();

    private:
        static void seek(Transform &transform, Velocity &velocity, Pathfinder &pathfinder);
    };
}

