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
    image = new rg::Surface((int) width, (int) height);
    image->Fill(rl::BLANK);
    rg::draw::rect(image, COLORS["pure white"], image->GetRect(), 0, 4);
    image->Blit(
            &text_surf, text_surf.GetRect().center(rg::math::Vector2{width / 2, height / 2}).pos);

    rect = image->GetRect().midbottom(character->rect.midtop() + rg::math::Vector2{0, -10});
}

DialogSprite::DialogSprite(DialogSprite &&other) noexcept
    : DialogSprite()
{
    *this = std::move(other);
}

DialogSprite &DialogSprite::operator=(DialogSprite &&other) noexcept
{
    if (this != &other)
    {
        Sprite::operator=(std::move(other));
        // DialogSprite creates its own image in constructor
        other.image = nullptr;
    }
    return *this;
}

DialogSprite::~DialogSprite()
{
    delete image;
}

TransitionSprite::TransitionSprite(
        const rg::math::Vector2 pos, rg::math::Vector2 size,
        std::pair<std::string, std::string> target)
    : Sprite(pos, new rg::Surface(size)), target(std::move(target))
{
}

TransitionSprite::TransitionSprite(TransitionSprite &&other) noexcept
    : TransitionSprite()
{
    *this = std::move(other);
}

TransitionSprite &TransitionSprite::operator=(TransitionSprite &&other) noexcept
{
    if (this != &other)
    {
        Sprite::operator=(std::move(other));
        target = std::move(other.target);
        // TransitionSprite creates its own image in constructor
        other.image = nullptr;
    }
    return *this;
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
    : monster(monster), index(index), pos_index(pos_index), entity(entity),
      apply_attack(apply_attack), createMonster(createMonster), frames(frames)
{
    image = &(*frames)[state];
    auto *tmp_frames = dynamic_cast<rg::Frames *>(image);
    tmp_frames->SetAtlas();

    highlight_mask = rg::mask::FromSurface(tmp_frames).ToFrames(
            tmp_frames->m_rows, tmp_frames->m_cols);
    highlight_mask.Fill(rl::BLANK);
    highlight_mask.frames = tmp_frames->frames;

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

MonsterSprite::MonsterSprite(MonsterSprite &&other) noexcept
    : MonsterSprite()
{
    *this = std::move(other);
}

MonsterSprite &MonsterSprite::operator=(MonsterSprite &&other) noexcept
{
    if (this != &other)
    {
        Sprite::operator=(std::move(other));
        monster = other.monster;
        state = other.state;
        highlight_mask = std::move(other.highlight_mask);
        frame_index = other.frame_index;
        index = other.index;
        pos_index = other.pos_index;
        entity = other.entity;
        frames = other.frames;
        animation_speed = other.animation_speed;
        highlight = other.highlight;
        target_sprite = other.target_sprite;
        current_attack = other.current_attack;
        apply_attack = other.apply_attack;
        newMonster = other.newMonster;
        newIndex = other.newIndex;
        newPosIndex = other.newPosIndex;
        newSide = other.newSide;
        createMonster = other.createMonster;

        timers["remove_highlight"].func = [this]()
        {
            SetHighlight(false);
        };
        timers["kill"].func = [this]()
        {
            Destroy();
        };
    }
    return *this;
}

void MonsterSprite::Update(const float deltaTime)
{
    monster->Update(deltaTime);
    for (auto &[key, timer]: timers)
    {
        timer.Update();
    }
    Animate(deltaTime);
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

    if (highlight) // blink
    {
        image = &highlight_mask;
    }

    dynamic_cast<rg::Frames *>(image)->SetAtlas(int(frame_index));
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

    image = new rg::Surface(
            (int) text_surf.GetRect().width + padding * 2,
            (int) text_surf.GetRect().height + padding * 2);
    image->Fill(COLORS["white"]);
    image->Blit(&text_surf, rg::math::Vector2{padding, padding});
    rect = image->GetRect().midtop(pos);
    z = BATTLE_LAYERS["name"];
}

MonsterNameSprite::MonsterNameSprite(MonsterNameSprite &&other) noexcept
    : MonsterNameSprite()
{
    *this = std::move(other);
}

MonsterNameSprite &MonsterNameSprite::operator=(MonsterNameSprite &&other) noexcept
{
    if (this != &other)
    {
        Sprite::operator=(std::move(other));
        monster_sprite = other.monster_sprite;
        // MonsterNameSprite creates its own image in constructor
        other.image = nullptr;
        other.monster_sprite = nullptr;
    }
    return *this;
}

MonsterNameSprite::~MonsterNameSprite()
{
    delete image;
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

MonsterLevelSprite::MonsterLevelSprite(MonsterLevelSprite &&other) noexcept
    : MonsterLevelSprite()
{
    *this = std::move(other);
}

MonsterLevelSprite &MonsterLevelSprite::operator=(MonsterLevelSprite &&other) noexcept
{
    if (this != &other)
    {
        Sprite::operator=(std::move(other));
        monster_sprite = other.monster_sprite;
        font = other.font;
        xp_rect = other.xp_rect;
        // MonsterLevelSprite creates its own image in constructor
        other.image = nullptr;
        other.monster_sprite = nullptr;
        other.font = nullptr;
    }
    return *this;
}

MonsterLevelSprite::~MonsterLevelSprite()
{
    delete image;
}

void MonsterLevelSprite::Update(float deltaTime)
{
    if (monster_sprite->Groups().empty())
    {
        Kill();
        return;
    }

    image->Fill(COLORS["white"]);

    const auto text_surf = font->render(
            rl::TextFormat("Lvl: %d", monster_sprite->monster->level), COLORS["black"]);
    const auto text_rect = text_surf.GetRect().center({rect.width / 2, rect.height / 2});
    image->Blit(&text_surf, text_rect);

    rg::draw::bar(
            image, xp_rect, monster_sprite->monster->xp, monster_sprite->monster->level_up,
            COLORS["black"], COLORS["white"]);
}

MonsterStatsSprite::MonsterStatsSprite(
        const rg::math::Vector2 pos, MonsterSprite *monster_sprite,
        const rg::math::Vector2 size, const rg::font::Font *font)
    : monster_sprite(monster_sprite), font(font)
{
    image = new rg::Surface(size);
    rect = image->GetRect().midbottom(pos);
    z = BATTLE_LAYERS["overlay"];
}

MonsterStatsSprite::MonsterStatsSprite(MonsterStatsSprite &&other) noexcept
    : MonsterStatsSprite()
{
    *this = std::move(other);
}

MonsterStatsSprite &MonsterStatsSprite::operator=(MonsterStatsSprite &&other) noexcept
{
    if (this != &other)
    {
        Sprite::operator=(std::move(other));
        monster_sprite = other.monster_sprite;
        font = other.font;
        other.image = nullptr;
        other.monster_sprite = nullptr;
        other.font = nullptr;
    }
    return *this;
}

MonsterStatsSprite::~MonsterStatsSprite()
{
    delete image;
}

void MonsterStatsSprite::Update(float deltaTime)
{
    if (monster_sprite->Groups().empty())
    {
        Kill();
        return;
    }

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
    if (monster_sprite->Groups().empty())
    {
        Kill();
        return;
    }

    image = &(*frames)[monster_sprite->state];
    dynamic_cast<rg::Frames *>(image)->SetAtlas(int(this->monster_sprite->frame_index));
    rect = dynamic_cast<rg::Frames *>(image)->GetRect().center(
            this->monster_sprite->rect.center());
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
