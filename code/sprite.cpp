#include "sprite.h"
#include "settings.h"


Sprite::Sprite(const rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf, const int z)
{
    image = surf;
    rect = image->GetRect();
    rect.topleft(pos);
    this->z = z;
    y_sort = rect.centery();
    hitbox = rect;
}

AnimatedSprite::AnimatedSprite(
        const rg::math::Vector2 pos, const std::shared_ptr<rg::Frames> &surf, const int z)
    : Sprite(pos, surf, z), image(surf)
{}

void AnimatedSprite::Update(const float deltaTime)
{
    Animate(deltaTime);
}

void AnimatedSprite::Animate(const float dt)
{
    frame_index += ANIMATION_SPEED * dt;
    if (frame_index > image->frames.size())
    {
        frame_index = 0;
    }
    image->SetAtlas(int(frame_index));
}

MonsterPatchSprite::MonsterPatchSprite(
        const rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf,
        const std::string &biome)
    : Sprite(pos, surf, WORLD_LAYERS["main"]), biome(biome)
{
    y_sort -= 40;
    // move sand patches to background drawing layer
    if (!std::strcmp(biome.c_str(), "sand"))
    {
        z = WORLD_LAYERS["bg"];
    }
}

BorderSprite::BorderSprite(const rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf)
    : Sprite(pos, surf)
{}

CollidableSprite::CollidableSprite(
        const rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf)
    : Sprite(pos, surf)
{
    hitbox = rect.inflate(0, -rect.height * 0.6f);
}

DialogSprite::DialogSprite(
        const std::string &message, const std::shared_ptr<Character> &character,
        const std::shared_ptr<rg::font::Font> &font)
    : message(message), character(character), font(font)
{
    z = WORLD_LAYERS["top"];

    // text
    auto text_surf = this->font->render(message.c_str(), COLORS["black"]);
    image = text_surf;
    rect = image->GetRect().midbottom(character->rect.midtop() + rg::math::Vector2{0, -10});
}
