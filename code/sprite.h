#pragma once
#include <vector>
#include <raylib.h>
#include "settings.h"
#include "tiledtexture.h"

class SpriteGroup;

class SimpleSprite
{
public:


    explicit SimpleSprite(SpriteGroup *sprite_group);
    virtual ~SimpleSprite() = default;

    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime);
    SpriteType type{SIMPLESPRITE};

protected:

    Rectangle rect{};
    TiledTexture image{};

private:

    std::vector<SpriteGroup *> groups;
};

class Sprite : public SimpleSprite
{
public:

    Sprite(Vector2 pos, const TiledTexture &img, SpriteGroup *sg, int z_ = WORLD_LAYERS["main"]);

    int z = WORLD_LAYERS["main"];
    int y_sort{};

private:

    std::vector<SpriteGroup *> groups;
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(
            Vector2 position, const std::vector<TiledTexture> &frms, SpriteGroup *sprite_group,
            int z = WORLD_LAYERS["main"]);
    void Animate(double deltaTime);
    void Update(double deltaTime) override;

private:

    double frame_index;
    std::vector<TiledTexture> frames;
};

int GetZ(const SimpleSprite *sprite);

class SpriteGroup
{
public:

    virtual ~SpriteGroup() = default;
    virtual void Draw() const;
    void Update(double deltaTime);
    std::vector<SimpleSprite *> sprites;
};
