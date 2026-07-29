#include "sprite_utils.h"

#include "ecs/ecs.h"

namespace idrs
{
    void SpriteUtils::constructSprite(Transform &transform, Sprite &sprite)
    {
        if (sprite.material.texture)
        {
            sprite.textureRect.x /= sprite.material.texture->getWidth();
            sprite.textureRect.w /= sprite.material.texture->getWidth();
            sprite.textureRect.y /= sprite.material.texture->getHeight();
            sprite.textureRect.h /= sprite.material.texture->getHeight();
        }
        sprite.vertices.append(idrs::Vertex{});
        sprite.vertices.append(idrs::Vertex{});
        sprite.vertices.append(idrs::Vertex{});
        sprite.vertices.append(idrs::Vertex{});

        updateDrawPosition(transform, sprite);
        if (sprite.material.texture)
        {
            setTextureCoords(sprite,
                             sprite.textureRect.x,
                             sprite.textureRect.y,
                             sprite.textureRect.w,
                             sprite.textureRect.h);

        }

        sprite.vertices.append({ 0, 1, 2, 2, 1, 3 });
        sprite.vertices.update();
    }

    void SpriteUtils::setTextureCoords(Sprite &sprite, f32 x, f32 y, f32 w, f32 h)
    {
        sprite.vertices[0].uv = { x, y };
        sprite.vertices[1].uv = { x + w, y };
        sprite.vertices[2].uv = { x, y + h };
        sprite.vertices[3].uv = { x + w, y + h };
    }

    void SpriteUtils::updateDrawPosition(Transform &transform, Sprite &sprite)
    {
        f32 scaledWidth  = sprite.width  * transform.scale.x;
        f32 scaledHeight = sprite.height * transform.scale.y;

        sprite.vertices[0].position = transform.position;
        sprite.vertices[1].position = { transform.position.x + scaledWidth, transform.position.y };
        sprite.vertices[2].position = { transform.position.x, transform.position.y + scaledHeight };
        sprite.vertices[3].position = { transform.position.x + scaledWidth, transform.position.y + scaledHeight };

        sprite.vertices.update();
    }

    void SpriteUtils::flipX(Entity entity)
    {
        Sprite &sprite = ECS::get<Sprite>(entity);

        sprite.vertices[0].uv.x = sprite.textureRect.w - sprite.vertices[0].uv.x;
        sprite.vertices[1].uv.x = sprite.textureRect.w - sprite.vertices[1].uv.x;
        sprite.vertices[2].uv.x = sprite.textureRect.w - sprite.vertices[2].uv.x;
        sprite.vertices[3].uv.x = sprite.textureRect.w - sprite.vertices[3].uv.x;

        sprite.flipx = !sprite.flipx;
    }

    void SpriteUtils::flipY(Entity entity)
    {
        Sprite &sprite = ECS::get<Sprite>(entity);

        sprite.vertices[0].uv.y = sprite.textureRect.y - sprite.vertices[0].uv.y;
        sprite.vertices[1].uv.y = sprite.textureRect.y - sprite.vertices[1].uv.y;
        sprite.vertices[2].uv.y = sprite.textureRect.y - sprite.vertices[2].uv.y;
        sprite.vertices[3].uv.y = sprite.textureRect.y - sprite.vertices[3].uv.y;

        sprite.flipy = !sprite.flipy;
    }
}
