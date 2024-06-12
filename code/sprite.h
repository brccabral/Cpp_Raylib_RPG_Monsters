#pragma once
#include "tiledtexture.h"
#include <raylib.h>
#include <vector>

class SpriteGroup;

class SimpleSprite
{
public:


    explicit SimpleSprite(SpriteGroup *sprite_group);
    virtual ~SimpleSprite() = default;
    ;
    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime);
    int SpriteType{0};

protected:

    Vector2 position{};
    TiledTexture tiled_texture{};

private:

    std::vector<SpriteGroup *> groups;
};

class Sprite : public SimpleSprite
{
public:

    Sprite(Vector2 pos, const TiledTexture &tt, SpriteGroup *sg);

private:

    std::vector<SpriteGroup *> groups;
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(Vector2 position, const std::vector<TiledTexture> &tts, SpriteGroup *sprite_group);
    void Animate(double deltaTime);
    void Update(double deltaTime) override;

private:

    double frame_index;
    std::vector<TiledTexture> frames;
};

class SpriteGroup
{
public:

    virtual ~SpriteGroup() = default;
    void Draw() const;
    void Update(double deltaTime);
    std::vector<SimpleSprite *> sprites;
};
