#pragma once
#include "entities.h"


#include <rygame.h>
#include "settings.h"

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
