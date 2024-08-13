#pragma once
#include <rygame.h>
#include "settings.h"

class Sprite : public rg::sprite::Sprite
{
public:

    Sprite(rg::math::Vector2 pos, const std::shared_ptr<rg::Surface> &surf,
           int z = WORLD_LAYERS["main"]);
    float y_sort;
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
