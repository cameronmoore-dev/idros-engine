#include "pathfindingsystem.h"

#include "ecs/ecs.h"

namespace idrs
{
    void PathfindingSystem::update()
    {
        ECS::query<Transform, Velocity, Pathfinder>([](Entity e, auto &transform, auto &velocity, auto &pathfinder)
        {
            switch (pathfinder.steerBehaviour)
            {
                case SEEK: seek(transform, velocity, pathfinder); break;
            }
        });
    }

    void PathfindingSystem::seek(Transform &transform, Velocity &velocity, Pathfinder &pathfinder)
    {
        Vec3f &targetPos = ECS::get<Transform>(pathfinder.target).position;

        Vec3f desiredVelocity = math::normalize(targetPos - transform.position);
        Vec3f steerForce;
        steerForce.x = (desiredVelocity.x - velocity.x) * pathfinder.smoothing;
        steerForce.y = (desiredVelocity.y - velocity.y) * pathfinder.smoothing;

        velocity.x += steerForce.x;
        velocity.y += steerForce.y;
    }
}
