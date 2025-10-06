#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "monster.hpp"
#include "settings.hpp"


// overworld sprites
class Sprite : public rg::sprite::Sprite
{
public:

    Sprite() = default;
    Sprite(
            rg::math::Vector2<float> pos, rg::Surface *surf,
            int z = Settings::GetInstance().WORLD_LAYERS["main"]);
    float y_sort{};
    rg::Rect hitbox{};
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite() = default;
    AnimatedSprite(
            rg::math::Vector2<float> pos, rg::Frames *surf,
            int z = Settings::GetInstance().WORLD_LAYERS["main"]);

    void Update(float deltaTime) override;
    virtual void Animate(float dt);

    rg::Frames *frames{};

protected:

    float frame_index{};
};

class MonsterPatchSprite : public Sprite
{
public:

    MonsterPatchSprite(
            rg::math::Vector2<float> pos, rg::Surface *surf, const std::string &biome,
            const std::string &patch_monsters, int level);

    std::string biome;
    std::vector<std::string> monsters{};
    int level;
};

class BorderSprite : public Sprite
{
public:

    BorderSprite(rg::math::Vector2<float> pos, rg::Surface *surf);
};

class CollidableSprite : public Sprite
{
public:

    CollidableSprite(rg::math::Vector2<float> pos, rg::Surface *surf);
};

class DialogSprite : public rg::sprite::Sprite
{
public:

    DialogSprite() = default;
    DialogSprite(
            const std::string &message, const Character *character,
            const rg::font::Font *font);
    DialogSprite(const DialogSprite &other) = delete;
    DialogSprite &operator=(const DialogSprite &other) = delete;
    DialogSprite(DialogSprite &&other) noexcept;
    DialogSprite &operator=(DialogSprite &&other) noexcept;
    ~DialogSprite() override;

    rg::Surface text_surf{};

};

class TransitionSprite : public Sprite
{
public:

    TransitionSprite() = default;
    TransitionSprite(
            rg::math::Vector2<float> pos, rg::math::Vector2<float> size,
            std::pair<std::string, std::string> target);
    TransitionSprite(const TransitionSprite &other) = delete;
    TransitionSprite &operator=(const TransitionSprite &other) = delete;
    TransitionSprite(TransitionSprite &&other) noexcept;
    TransitionSprite &operator=(TransitionSprite &&other) noexcept;
    ~TransitionSprite() override;

    std::pair<std::string, std::string> target{};
};

// battle sprites
class MonsterSprite : public rg::sprite::Sprite
{
public:

    MonsterSprite() = default;
    MonsterSprite(
            rg::math::Vector2<float> pos, std::unordered_map<AnimationState, rg::Frames> *frames,
            Monster *monster, int index, int pos_index, SelectionSide entity,
            const std::function<
                void(
                        const MonsterSprite *target_sprite, Attack attack,
                        float amount)> &apply_attack,
            const std::function<void(
                    Monster *monster, int index, int pos_index, SelectionSide entity)>
            &createMonster);
    MonsterSprite(const MonsterSprite &other) = delete;
    MonsterSprite &operator=(const MonsterSprite &other) = delete;
    MonsterSprite(MonsterSprite &&other) noexcept;
    MonsterSprite &operator=(MonsterSprite &&other) noexcept;

    void Update(float deltaTime) override;
    void SetHighlight(bool value);
    void
    ActivateAttack(const MonsterSprite *monster_sprite, Attack selected_attack);
    void DelayedKill(
            Monster *monster_, int index_, int pos_index_,
            SelectionSide side);

    Monster *monster{};
    AnimationState state = ANIMATIONSTATE_IDLE;
    rg::Frames highlight_mask{};
    float frame_index{};
    int index{};
    int pos_index{};
    SelectionSide entity{};

    std::function<void(
            const MonsterSprite *target_sprite, Attack attack, float amount)>
    apply_attack{};
    std::function<void(
            Monster *monster, int index, int pos_index, SelectionSide entity)>
    createMonster{};

private:

    void Animate(float dt);
    void Destroy();

    std::unordered_map<AnimationState, rg::Frames> *frames{};

    float animation_speed{};
    bool highlight{};
    std::unordered_map<std::string, rg::Timer> timers{};

    const MonsterSprite *target_sprite = nullptr;
    Attack current_attack = ATTACK_NONE;

    Monster *newMonster = nullptr;
    int newIndex = 0;
    int newPosIndex = 0;
    SelectionSide newSide{};
};

class MonsterNameSprite : public rg::sprite::Sprite
{
public:

    MonsterNameSprite() = default;
    MonsterNameSprite(
            rg::math::Vector2<float> pos, MonsterSprite *monster_sprite,
            const rg::font::Font *font);
    MonsterNameSprite(const MonsterNameSprite &other) = delete;
    MonsterNameSprite &operator=(const MonsterNameSprite &other) = delete;
    MonsterNameSprite(MonsterNameSprite &&other) noexcept;
    MonsterNameSprite &operator=(MonsterNameSprite &&other) noexcept;
    ~MonsterNameSprite() override;

    void Update(float deltaTime) override;

private:

    MonsterSprite *monster_sprite{};
    rg::Surface text_surf{};
};

class MonsterLevelSprite : public rg::sprite::Sprite
{
public:

    MonsterLevelSprite() = default;
    MonsterLevelSprite(
            SelectionSide entity, rg::math::Vector2<float> anchor,
            MonsterSprite *monster_sprite,
            const rg::font::Font *font);
    MonsterLevelSprite(const MonsterLevelSprite &other) = delete;
    MonsterLevelSprite &operator=(const MonsterLevelSprite &other) = delete;
    MonsterLevelSprite(MonsterLevelSprite &&other) noexcept;
    MonsterLevelSprite &operator=(MonsterLevelSprite &&other) noexcept;
    ~MonsterLevelSprite() override;
    void Update(float deltaTime) override;

private:

    MonsterSprite *monster_sprite{};
    const rg::font::Font *font{};
    rg::Rect xp_rect{};
    rg::Surface text_surf{};
};

class MonsterStatsSprite : public rg::sprite::Sprite
{
public:

    MonsterStatsSprite() = default;
    MonsterStatsSprite(
            rg::math::Vector2<float> pos, MonsterSprite *monster_sprite,
            rg::math::Vector2<float> size, const rg::font::Font *font);
    MonsterStatsSprite(const MonsterStatsSprite &other) = delete;
    MonsterStatsSprite &operator=(const MonsterStatsSprite &other) = delete;
    MonsterStatsSprite(MonsterStatsSprite &&other) noexcept;
    MonsterStatsSprite &operator=(MonsterStatsSprite &&other) noexcept;
    ~MonsterStatsSprite() override;
    void Update(float deltaTime) override;

private:

    MonsterSprite *monster_sprite{};
    const rg::font::Font *font{};
    rg::Surface text_surf{};
};

class MonsterOutlineSprite : public rg::sprite::Sprite
{
public:

    MonsterOutlineSprite() = default;
    MonsterOutlineSprite(
            MonsterSprite *monster_sprite,
            std::unordered_map<AnimationState, rg::Frames> *frames);
    void Update(float deltaTime) override;

    MonsterSprite *monster_sprite{};

private:

    std::unordered_map<AnimationState, rg::Frames> *frames{};
};

class AttackSprite : public AnimatedSprite
{
public:

    AttackSprite() = default;
    AttackSprite(
            rg::math::Vector2<float> position, rg::Frames *frames,
            int z = Settings::GetInstance().BATTLE_LAYERS["overlay"]);
    void Animate(float dt) override;
    bool active{};
};

class TimedSprite : public Sprite
{
public:

    TimedSprite() = default;
    TimedSprite(rg::math::Vector2<float> pos, rg::Surface *surf, float duration);
    TimedSprite(const TimedSprite &other) = delete;
    TimedSprite &operator=(const TimedSprite &other) = delete;
    TimedSprite(TimedSprite &&other) noexcept;
    TimedSprite &operator=(TimedSprite &&other) noexcept;

    void Update(float deltaTime) override;

    rg::Timer death_timer;
};
