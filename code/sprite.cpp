#include <utility>
#include <array>
#include "sprite.hpp"
#include "settings.hpp"


Sprite::Sprite(const rg::math::Vector2 pos, rg::Surface *surf, const int z)
{
    image = surf;
    rect = image->GetRect();
    rect.topleft(pos);
    this->z = z;
    y_sort = rect.centery();
    hitbox = rect;
}

AnimatedSprite::AnimatedSprite(const rg::math::Vector2 pos, rg::Frames *surf, const int z)
    : Sprite(pos, surf, z), image(surf)
{
}

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
        const rg::math::Vector2 pos, rg::Surface *surf, const std::string &biome,
        const std::string &patch_monsters, const int level)
    : Sprite(pos, surf, WORLD_LAYERS["main"]), biome(biome), level(level)
{
    y_sort -= 40;
    // move sand patches to background drawing layer
    if (!std::strcmp(biome.c_str(), "sand"))
    {
        z = WORLD_LAYERS["bg"];
    }
    monsters = rg::Split(patch_monsters, ',');
}

BorderSprite::BorderSprite(const rg::math::Vector2 pos, rg::Surface *surf)
    : Sprite(pos, surf)
{
}

CollidableSprite::CollidableSprite(const rg::math::Vector2 pos, rg::Surface *surf)
    : Sprite(pos, surf)
{
    hitbox = rect.inflate(0, -rect.height * 0.6f);
}

DialogSprite::DialogSprite(
        const std::string &message, const Character *character,
        const rg::font::Font *font)
{
    z = WORLD_LAYERS["top"];

    // text
    const auto text_surf = font->render(message.c_str(), COLORS["black"]);
    constexpr int padding = 5;
    float width = std::max(30.0f, text_surf.GetRect().width + padding * 2);
    float height = text_surf.GetRect().height + padding * 2;

    // background
    auto surf = rg::Surface((int) width, (int) height);
    surf.Fill(rl::BLANK);
    rg::draw::rect(&surf, COLORS["pure white"], surf.GetRect(), 0, 4);
    surf.Blit(
            &text_surf, text_surf.GetRect().center(rg::math::Vector2{width / 2, height / 2}).pos);

    *image = std::move(surf);
    rect = image->GetRect().midbottom(character->rect.midtop() + rg::math::Vector2{0, -10});
}

TransitionSprite::TransitionSprite(
        const rg::math::Vector2 pos, rg::math::Vector2 size,
        std::pair<std::string, std::string> target)
    : Sprite(pos, new rg::Surface(size)), target(std::move(target))
{
}

TransitionSprite::~TransitionSprite()
{
    delete image;
}

MonsterSprite::MonsterSprite(
        const rg::math::Vector2 pos, std::unordered_map<AnimationState, rg::Frames> *frames,
        Monster *monster, const int index, const int pos_index, const SelectionSide entity,
        const std::function<
            void(
                    const MonsterSprite *target_sprite, Attack attack,
                    float amount)> &apply_attack,
        const std::function<void(
                Monster *monster, int index, int pos_index, SelectionSide entity)>
        &createMonster)
    : monster(monster), index(index), pos_index(pos_index), entity(entity), frames(frames),
      apply_attack(apply_attack), createMonster(createMonster)
{
    image = &(*frames)[state];
    dynamic_cast<rg::Frames *>(image)->SetAtlas();
    rect = dynamic_cast<rg::Frames *>(image)->GetRect().center(pos);
    animation_speed = (float) ANIMATION_SPEED + rg::math::get_random_uniform(-1, 1);
    z = BATTLE_LAYERS["monster"];
    timers["remove_highlight"] = rg::Timer(
            0.5f, false, false, [this]
            {
                SetHighlight(false);
            });
    timers["kill"] = rg::Timer(
            0.6f, false, false, [this]
            {
                Destroy();
            });
}

void MonsterSprite::Update(const float deltaTime)
{
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }
    Animate(deltaTime);
    monster->Update(deltaTime);
}

// blink monster with a white mask
void MonsterSprite::SetHighlight(const bool value)
{
    highlight = value;
    if (highlight)
    {
        timers["remove_highlight"].Activate();
    }
}

void MonsterSprite::ActivateAttack(
        const MonsterSprite *monster_sprite, const Attack selected_attack)
{
    state = ANIMATIONSTATE_ATTACK;
    frame_index = 0;
    target_sprite = monster_sprite;
    current_attack = selected_attack;
    monster->ReduceEnergy(selected_attack);
}

void MonsterSprite::DelayedKill(
        Monster *monster_, const int index_, const int pos_index_,
        const SelectionSide side)
{
    if (!timers["kill"].active)
    {
        newMonster = monster_;
        newIndex = index_;
        newPosIndex = pos_index_;
        newSide = side;
        timers["kill"].Activate();
    }
}

void MonsterSprite::Animate(const float dt)
{
    frame_index += ANIMATION_SPEED * dt;
    // attack animation has finished
    if (state == ANIMATIONSTATE_ATTACK &&
        frame_index >= dynamic_cast<rg::Frames *>(image)->frames.size())
    {
        // apply attack
        apply_attack(target_sprite, current_attack, monster->GetBaseDamage(current_attack));
        state = ANIMATIONSTATE_IDLE;
    }

    image = &(*frames)[state];
    dynamic_cast<rg::Frames *>(image)->SetAtlas(int(frame_index));

    if (highlight) // blink
    {
        const auto *frames_ = dynamic_cast<rg::Frames *>(image);
        auto mask_surf =
                rg::mask::FromSurface(frames_).ToFrames(frames_->m_rows, frames_->m_cols);
        mask_surf.Fill(rl::BLANK);
        mask_surf.frames = frames_->frames;
        *(dynamic_cast<rg::Frames *>(image)) = std::move(mask_surf);
    }
}

void MonsterSprite::Destroy()
{
    if (newMonster)
    {
        createMonster(newMonster, newIndex, newPosIndex, newSide);
        newMonster = nullptr;
        monster = nullptr;
    }
    target_sprite = nullptr;
    Kill();
}

MonsterNameSprite::MonsterNameSprite(
        const rg::math::Vector2 pos, MonsterSprite *monster_sprite,
        const rg::font::Font *font)
    : monster_sprite(monster_sprite)
{
    const auto text_surf = font->render(monster_sprite->monster->name.c_str(), COLORS["black"]);
    constexpr int padding = 10;

    *image = rg::Surface(
            (int) text_surf.GetRect().width + padding * 2,
            (int) text_surf.GetRect().height + padding * 2);
    image->Fill(COLORS["white"]);
    image->Blit(&text_surf, rg::math::Vector2{padding, padding});
    rect = image->GetRect().midtop(pos);
    z = BATTLE_LAYERS["name"];
}

void MonsterNameSprite::Update(float deltaTime)
{
    if (monster_sprite->Groups().empty())
    {
        Kill();
    }
}

MonsterLevelSprite::MonsterLevelSprite(
        SelectionSide entity, const rg::math::Vector2 anchor,
        MonsterSprite *monster_sprite,
        const rg::font::Font *font)
    : monster_sprite(monster_sprite), font(font)
{
    image = new rg::Surface(60, 26);
    if (entity == PLAYER)
    {
        rect = image->GetRect().topleft(anchor);
    }
    else
    {
        rect = image->GetRect().topright(anchor);
    }
    xp_rect = rg::Rect{0, rect.height - 2, rect.width, 2};
    z = BATTLE_LAYERS["name"];
}

MonsterLevelSprite::~MonsterLevelSprite()
{
    delete image;
}

void MonsterLevelSprite::Update(float deltaTime)
{
    image->Fill(COLORS["white"]);

    const auto text_surf = font->render(
            rl::TextFormat("Lvl: %d", monster_sprite->monster->level), COLORS["black"]);
    const auto text_rect = text_surf.GetRect().center({rect.width / 2, rect.height / 2});
    image->Blit(&text_surf, text_rect);

    rg::draw::bar(
            image, xp_rect, monster_sprite->monster->xp, monster_sprite->monster->level_up,
            COLORS["black"], COLORS["white"]);

    if (monster_sprite->Groups().empty())
    {
        Kill();
    }
}

MonsterStatsSprite::MonsterStatsSprite(
        const rg::math::Vector2 pos, MonsterSprite *monster_sprite,
        rg::math::Vector2 size, const rg::font::Font *font)
    : monster_sprite(monster_sprite), font(font)
{
    image = new rg::Surface(size);
    rect = image->GetRect().midbottom(pos);
    z = BATTLE_LAYERS["overlay"];
}

MonsterStatsSprite::~MonsterStatsSprite()
{
    delete image;
}

void MonsterStatsSprite::Update(float deltaTime)
{
    image->Fill(COLORS["white"]);

    const auto info = monster_sprite->monster->GetInfo();
    const std::array<rl::Color, 3> colors = {COLORS["red"], COLORS["blue"], COLORS["gray"]};
    for (unsigned int index = 0; index < info.size(); ++index)
    {
        const auto [value, max_value] = info[index];
        const auto color = colors[index];
        if (index < 2) // health and energy
        {
            const auto text_surf =
                    font->render(rl::TextFormat("%.f/%.f", value, max_value), COLORS["black"]);
            const auto text_rect =
                    text_surf.GetRect().topleft({rect.width * 0.05f, index * rect.height / 2});
            const auto bar_rect = rg::Rect{
                    text_rect.bottomleft() + rg::math::Vector2{0, -2}, {rect.width * 0.9f, 4}};

            image->Blit(&text_surf, text_rect);
            rg::draw::bar(image, bar_rect, value, max_value, color, COLORS["black"], 2);
        }
        else // initiative
        {
            const auto init_rect = rg::Rect{0, rect.height - 2, rect.width, 2.0f};
            rg::draw::bar(image, init_rect, value, max_value, color, COLORS["white"]);
        }
    }

    if (monster_sprite->Groups().empty())
    {
        Kill();
    }
}

MonsterOutlineSprite::MonsterOutlineSprite(
        MonsterSprite *monster_sprite,
        std::unordered_map<AnimationState, rg::Frames> *frames)
    : monster_sprite(monster_sprite), frames(frames)
{
    z = BATTLE_LAYERS["outline"];

    image = &(*frames)[monster_sprite->state];
    dynamic_cast<rg::Frames *>(image)->SetAtlas(int(this->monster_sprite->frame_index));
    rect = dynamic_cast<rg::Frames *>(image)->GetRect().center(
            this->monster_sprite->rect.center());
}

void MonsterOutlineSprite::Update(float deltaTime)
{
    image = &(*frames)[monster_sprite->state];
    dynamic_cast<rg::Frames *>(image)->SetAtlas(int(this->monster_sprite->frame_index));
    rect = dynamic_cast<rg::Frames *>(image)->GetRect().center(
            this->monster_sprite->rect.center());

    if (monster_sprite->Groups().empty())
    {
        Kill();
    }
}

AttackSprite::AttackSprite(
        const rg::math::Vector2 position, rg::Frames *frames, const int z)
    : AnimatedSprite(position, frames, z)
{
    rect.center(position);
    active = true;
}

void AttackSprite::Animate(const float dt)
{
    frame_index += ANIMATION_SPEED * dt;
    if (frame_index < image->frames.size())
    {
        image->SetAtlas(int(frame_index));
    }
    else
    {
        active = false;
        Kill();
    }
}

TimedSprite::TimedSprite(
        const rg::math::Vector2 pos, rg::Surface *surf, const float duration)
    : Sprite(pos, surf, BATTLE_LAYERS["overlay"])
{
    rect.center(pos);
    death_timer = rg::Timer(
            duration, false, true, [this]
            {
                Kill();
            });
}

void TimedSprite::Update(float deltaTime)
{
    death_timer.Update();
}
