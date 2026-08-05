#pragma once

#include "ecs/entity.h"
#include "ecs/components.h"
#include "sprite_animation.h"

namespace idrs
{
    class SpriteAnimationSystem
    {
    public:
        static void update();
        static void play(Entity entity);
        static void stop(Entity entity);
        static void change(Entity entity, const std::string &name);
        static void change(Entity entity, SpriteAnimation &anim);

    private:
        static void updateCurrentFrame(SpriteAnimator &animator);
        static void animate(Sprite &sprite, SpriteAnimator &animator);
        static void oneShot(SpriteAnimator &animator);
        static void repeat(SpriteAnimator &animator);
        static void pingPong(SpriteAnimator &animator);
        static void checkSpriteFlip();

        static void onAnimationLoopComplete(SpriteAnimator &animator, f32 setFrame);
    };
}
