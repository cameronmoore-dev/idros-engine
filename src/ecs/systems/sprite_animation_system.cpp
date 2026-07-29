#include "sprite_animation_system.h"

#include "time.hpp"
#include "resource_manager.h"
#include "ecs/ecs.h"
#include "utils/sprite_utils.h"

namespace idrs
{
    void SpriteAnimationSystem::update()
    {
        ECS::view<Sprite, SpriteAnimator>([](Entity e, Sprite &sprite, SpriteAnimator &animator)
        {
            if (animator.playing)
            {
                updateCurrentFrame(animator);

                SpriteAnimation::LoopType type = animator.current->getLoopType();
                switch (type)
                {
                    case SpriteAnimation::LoopType::None:       oneShot(animator);  break;
                    case SpriteAnimation::LoopType::Repeat:     repeat(animator);   break;
                    case SpriteAnimation::LoopType::PingPong:   pingPong(animator); break;
                    default: break;
                }

                animate(sprite, animator);
            }
        });
    }
    
    void SpriteAnimationSystem::updateCurrentFrame(SpriteAnimator &animator)
    {
        if (animator.current->isReversing())
        {
            animator.currentFrame -= Time::deltaTime() * animator.current->getSpeed();
            return;
        }

        animator.currentFrame += Time::deltaTime() * animator.current->getSpeed();
    }

    void SpriteAnimationSystem::animate(Sprite &sprite, SpriteAnimator &animator)
    {
        f32 frame = std::floor(animator.currentFrame);
        Vec2u start = animator.current->getStartFrame();
        Vec2u offset = animator.current->getFrameOffsets()[(u32)frame];
        Vec2u framePos = { start.x + ((u32)sprite.width * offset.x), start.y + ((u32)sprite.height * offset.y) };

        f32 x = ((f32)framePos.x / sprite.material.texture->getWidth());
        f32 y = (((f32)sprite.material.texture->getHeight() - sprite.height) - (f32)framePos.y) / sprite.material.texture->getHeight();
        f32 w = sprite.textureRect.w;
        f32 h = sprite.textureRect.h;

        if (sprite.flipx)
        {
            x += sprite.textureRect.w;
            w = -sprite.textureRect.w;
        }
        if (sprite.flipy)
        {
            y += sprite.textureRect.h - (f32)framePos.y;
            h = -sprite.textureRect.h;
        }

        SpriteUtils::setTextureCoords(sprite, x, y, w, h);
    }

    void SpriteAnimationSystem::play(Entity entity)
    {
        ECS::get<SpriteAnimator>(entity).playing = true;
    }

    void SpriteAnimationSystem::stop(Entity entity)
    {
        ECS::get<SpriteAnimator>(entity).playing = false;
    }

    void SpriteAnimationSystem::change(Entity entity, const std::string &name)
    {
        ECS::get<SpriteAnimator>(entity).current = &ResourceManager::getResource<SpriteAnimation>(name);
    }

    void SpriteAnimationSystem::change(Entity entity, SpriteAnimation &anim)
    {
        ECS::get<SpriteAnimator>(entity).current = &anim;
    }

    void SpriteAnimationSystem::oneShot(SpriteAnimator &animator)
    {
        if (animator.currentFrame >= animator.current->getTotalFrames())
        {
            animator.playing = false;
        }
    }

    void SpriteAnimationSystem::repeat(SpriteAnimator &animator)
    {
        if (animator.current->isReversing())
        {
            if (animator.currentFrame <= 0.0f)
            {
                animator.currentFrame = (f32)(animator.current->getTotalFrames() - 1);
            }
            return;
        }

        if (animator.currentFrame >= animator.current->getTotalFrames())
        {
            animator.currentFrame = 0.0f;
        }
    }

    void SpriteAnimationSystem::pingPong(SpriteAnimator &animator)
    {
        if (animator.current->isReversing())
        {
            if (animator.currentFrame <= 0.0f)
            {
                animator.current->setReversing(false);
                animator.currentFrame = 0.0f;
            }
            return;
        }

        if (animator.currentFrame >= animator.current->getTotalFrames())
        {
            animator.current->setReversing(true);
            animator.currentFrame = (f32)(animator.current->getTotalFrames() - 1);
        }
    }
}
