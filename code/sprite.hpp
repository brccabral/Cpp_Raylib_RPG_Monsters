#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "monster.hpp"
#include "settings.hpp"


// overworld sprites
class Sprite : public rg::sprite::Sprite
{
public:

    Sprite(rg::math::Vector2 pos, const rg::Surface_Ptr &surf,
           int z = WORLD_LAYERS["main"]);
    float y_sort;
    rg::Rect hitbox{};
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(
            rg::math::Vector2 pos, const std::shared_ptr<rg::Frames> &surf,
            int z = WORLD_LAYERS["main"]);

    void Update(float deltaTime) override;
    virtual void Animate(float dt);

    std::shared_ptr<rg::Frames> image;

protected:

    float frame_index{};
};

class MonsterPatchSprite : public Sprite
{
public:

    MonsterPatchSprite(
            rg::math::Vector2 pos, const rg::Surface_Ptr &surf,
            const std::string &biome, const std::string &patch_monsters, int level);

    std::string biome;
    std::vector<std::string> monsters{};
    int level;
};

class BorderSprite : public Sprite
{
public:

    BorderSprite(rg::math::Vector2 pos, const rg::Surface_Ptr &surf);
};

class CollidableSprite : public Sprite
{
public:

    CollidableSprite(rg::math::Vector2 pos, const rg::Surface_Ptr &surf);
};

class DialogSprite : public rg::sprite::Sprite
{
public:

    DialogSprite(
            const std::string &message, const std::shared_ptr<Character> &character,
            const std::shared_ptr<rg::font::Font> &font);

private:

    std::string message;
    std::shared_ptr<Character> character;
    std::shared_ptr<rg::font::Font> font;
};

class TransitionSprite : public Sprite
{
public:

    TransitionSprite(
            rg::math::Vector2 pos, rg::math::Vector2 size,
            std::pair<std::string, std::string> target);

    std::pair<std::string, std::string> target;
};

// battle sprites
class MonsterSprite : public rg::sprite::Sprite
{
public:

    MonsterSprite(
            rg::math::Vector2 pos,
            const std::map<AnimationState, std::shared_ptr<rg::Frames>> &frames,
            const std::shared_ptr<Monster> &monster, int index, int pos_index, SelectionSide entity,
            const std::function<
                    void(const std::shared_ptr<MonsterSprite> &target_sprite, Attack attack,
                         float amount)> &apply_attack,
            const std::function<
                    void(std::shared_ptr<Monster> monster, int index, int pos_index,
                         SelectionSide entity)> &createMonster);
    void Update(float deltaTime) override;
    void SetHighlight(bool value);
    void
    ActivateAttack(const std::shared_ptr<MonsterSprite> &monster_sprite, Attack selected_attack);
    void DelayedKill(
            const std::shared_ptr<Monster> &monster, int index, int pos_index, SelectionSide side);

    std::shared_ptr<Monster> monster;
    AnimationState state = ANIMATIONSTATE_IDLE;
    float frame_index{};
    int index{};
    int pos_index{};
    SelectionSide entity;

private:

    void Animate(float dt);
    void Destroy();

    std::map<AnimationState, std::shared_ptr<rg::Frames>> frames;

    float animation_speed{};
    bool highlight{};
    std::map<std::string, rg::Timer> timers;

    std::shared_ptr<MonsterSprite> target_sprite = nullptr;
    Attack current_attack = ATTACK_NONE;

    const std::function<void(
            const std::shared_ptr<MonsterSprite> &target_sprite, Attack attack, float amount)>
            apply_attack;

    std::shared_ptr<Monster> newMonster = nullptr;
    int newIndex = 0;
    int newPosIndex = 0;
    SelectionSide newSide{};
    const std::function<void(
            std::shared_ptr<Monster> monster, int index, int pos_index, SelectionSide entity)>
            createMonster;
};

class MonsterNameSprite : public rg::sprite::Sprite
{
public:

    MonsterNameSprite(
            rg::math::Vector2 pos, const std::shared_ptr<MonsterSprite> &monster_sprite,
            const std::shared_ptr<rg::font::Font> &font);
    void Update(float deltaTime) override;

private:

    std::shared_ptr<MonsterSprite> monster_sprite;
};

class MonsterLevelSprite : public rg::sprite::Sprite
{
public:

    MonsterLevelSprite(
            SelectionSide entity, rg::math::Vector2 anchor,
            const std::shared_ptr<MonsterSprite> &monster_sprite,
            const std::shared_ptr<rg::font::Font> &font);
    void Update(float deltaTime) override;

private:

    std::shared_ptr<MonsterSprite> monster_sprite;
    std::shared_ptr<rg::font::Font> font;
    rg::Rect xp_rect{};
};

class MonsterStatsSprite : public rg::sprite::Sprite
{
public:

    MonsterStatsSprite(
            rg::math::Vector2 pos, const std::shared_ptr<MonsterSprite> &monster_sprite,
            rg::math::Vector2 size, const std::shared_ptr<rg::font::Font> &font);
    void Update(float deltaTime) override;

private:

    std::shared_ptr<MonsterSprite> monster_sprite;
    std::shared_ptr<rg::font::Font> font;
};

class MonsterOutlineSprite : public rg::sprite::Sprite
{
public:

    MonsterOutlineSprite(
            const std::shared_ptr<MonsterSprite> &monster_sprite,
            const std::map<AnimationState, std::shared_ptr<rg::Frames>> &frames);
    void Update(float deltaTime) override;

    std::shared_ptr<MonsterSprite> monster_sprite;

private:

    std::map<AnimationState, std::shared_ptr<rg::Frames>> frames;
};

class AttackSprite : public AnimatedSprite
{
public:

    AttackSprite(
            rg::math::Vector2 position, const std::shared_ptr<rg::Frames> &frames,
            int z = BATTLE_LAYERS["overlay"]);
    void Animate(float dt) override;
};

class TimedSprite : public Sprite
{
public:

    TimedSprite(rg::math::Vector2 pos, const rg::Surface_Ptr &surf, float duration);
    void Update(float deltaTime) override;

private:

    rg::Timer death_timer;
};
