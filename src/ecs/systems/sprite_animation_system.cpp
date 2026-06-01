#include "sprite_animation_system.h"

namespace idrs
{
    void SpriteAnimationSystem::update()
    {
        ECS::_view<Sprite, SpriteAnimator>([](Entity e, Sprite &sprite, SpriteAnimator &animator)
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
        Vec2i start = animator.current->getStartFrame();
        Vec2i offset = animator.current->getFrameOffsets()[(u32)frame];
        Vec2i framePos = { start.x + ((u32)sprite.width * offset.x), start.y + ((u32)sprite.height * offset.y) };

        sprite.textureRect.x = ((f32)framePos.x / sprite.texture->getWidth());
        sprite.textureRect.y = (((f32)sprite.texture->getHeight() - sprite.height) - (f32)framePos.y) / sprite.texture->getHeight();

        sprite.vertices[0].uv = { sprite.textureRect.x, sprite.textureRect.y };
        sprite.vertices[1].uv = { sprite.textureRect.x + sprite.textureRect.w, sprite.textureRect.y };
        sprite.vertices[2].uv = { sprite.textureRect.x, sprite.textureRect.y + sprite.textureRect.h };
        sprite.vertices[3].uv = { sprite.textureRect.x + sprite.textureRect.w, sprite.textureRect.y + sprite.textureRect.h };
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