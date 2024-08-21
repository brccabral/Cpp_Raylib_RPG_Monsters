#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "monster.hpp"
#include "settings.hpp"


// overworld sprites
class Sprite : public rg::sprite::Sprite
{
public:

    Sprite(rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf,
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
    void Animate(float dt);

    std::shared_ptr<rg::Frames> image;

private:

    float frame_index{};
};

class MonsterPatchSprite : public Sprite
{
public:

    MonsterPatchSprite(
            rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf,
            const std::string &biome);

private:

    std::string biome;
};

class BorderSprite : public Sprite
{
public:

    BorderSprite(rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf);
};

class CollidableSprite : public Sprite
{
public:

    CollidableSprite(rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf);
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
            rg::math::Vector2 pos, const std::map<std::string, std::shared_ptr<rg::Frames>> &frames,
            Monster *monster, int index, int pos_index, SelectionSide entity);
    void Update(float deltaTime) override;
    void SetHighlight(bool value);

    Monster *monster;
    std::string state = "idle";
    float frame_index{};

private:

    void Animate(float dt);

    int index{};
    int pos_index{};
    SelectionSide entity;

    std::map<std::string, std::shared_ptr<rg::Frames>> frames;

    float animation_speed{};
    bool highlight{};
    std::map<std::string, rg::Timer> timers;
};

class MonsterNameSprite : public rg::sprite::Sprite
{
public:

    MonsterNameSprite(
            rg::math::Vector2 pos, const std::shared_ptr<MonsterSprite> &monster_sprite,
            const std::shared_ptr<rg::font::Font> &font);

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
            const std::map<std::string, std::shared_ptr<rg::Frames>> &frames);
    void Update(float deltaTime) override;

    std::shared_ptr<MonsterSprite> monster_sprite;

private:

    std::map<std::string, std::shared_ptr<rg::Frames>> frames;
};
