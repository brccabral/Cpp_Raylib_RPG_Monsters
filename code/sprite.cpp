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
    const auto text_surf = this->font->render(message.c_str(), COLORS["black"]);
    constexpr int padding = 5;
    float width = std::max(30.0f, text_surf->GetRect().width + padding * 2);
    float height = text_surf->GetRect().height + padding * 2;

    // background
    const auto surf = std::make_shared<rg::Surface>(width, height);
    surf->Fill(rl::BLANK);
    rg::draw::rect(surf, COLORS["pure white"], surf->GetRect(), 0, 4);
    surf->Blit(
            text_surf, text_surf->GetRect().center(rg::math::Vector2{width / 2, height / 2}).pos);

    image = surf;
    rect = image->GetRect().midbottom(character->rect.midtop() + rg::math::Vector2{0, -10});
}
