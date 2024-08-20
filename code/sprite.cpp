#include <utility>
#include "sprite.hpp"
#include "settings.hpp"


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

TransitionSprite::TransitionSprite(
        const rg::math::Vector2 pos, rg::math::Vector2 size,
        std::pair<std::string, std::string> target)
    : Sprite(pos, std::make_shared<rg::Surface>(size)), target(std::move(target))
{}

MonsterSprite::MonsterSprite(
        const rg::math::Vector2 pos,
        const std::map<std::string, std::shared_ptr<rg::Frames>> &frames, Monster *monster,
        const int index, const int pos_index, const std::string &entity)
    : monster(monster), index(index), pos_index(pos_index), entity(entity), frames(frames)
{
    int p = this->index + this->pos_index + this->frame_index + this->monster->level;
    image = this->frames[state];
    std::dynamic_pointer_cast<rg::Frames>(image)->SetAtlas(p - p);
    rect = std::dynamic_pointer_cast<rg::Frames>(image)->GetRect().center(pos);
    animation_speed = (float) ANIMATION_SPEED + rg::math::get_random_uniform(-1, 1);
}

void MonsterSprite::Update(const float deltaTime)
{
    Animate(deltaTime);
}

void MonsterSprite::Animate(const float dt)
{
    frame_index += ANIMATION_SPEED * dt;
    image = frames[state];
    std::dynamic_pointer_cast<rg::Frames>(image)->SetAtlas(frame_index);
}

MonsterNameSprite::MonsterNameSprite(
        const rg::math::Vector2 pos, const std::shared_ptr<MonsterSprite> &monster_sprite,
        const std::shared_ptr<rg::font::Font> &font)
    : monster_sprite(monster_sprite)
{
    const auto text_surf = font->render(monster_sprite->monster->name.c_str(), COLORS["black"]);
    constexpr int padding = 10;

    image = std::make_shared<rg::Surface>(
            text_surf->GetRect().width + padding * 2, text_surf->GetRect().height + padding * 2);
    image->Fill(COLORS["white"]);
    image->Blit(text_surf, {padding, padding});
    rect = image->GetRect().midtop(pos);
}

MonsterLevelSprite::MonsterLevelSprite(
        const std::string &entity, const rg::math::Vector2 anchor,
        const std::shared_ptr<MonsterSprite> &monster_sprite,
        const std::shared_ptr<rg::font::Font> &font)
    : monster_sprite(monster_sprite), font(font)
{
    image = std::make_shared<rg::Surface>(60, 26);
    if (!std::strcmp(entity.c_str(), "player"))
    {
        rect = image->GetRect().topleft(anchor);
    }
    else
    {
        rect = image->GetRect().topright(anchor);
    }
}

void MonsterLevelSprite::Update(float deltaTime)
{
    image->Fill(COLORS["white"]);
}
