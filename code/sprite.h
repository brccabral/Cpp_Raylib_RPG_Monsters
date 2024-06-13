#pragma once
#include <vector>
#include <raylib.h>
#include "settings.h"


typedef struct TiledTexture
{
    Texture2D *texture;
    Rectangle rect;
} TiledTexture;

class SpriteGroup;

class SimpleSprite
{
public:


    explicit SimpleSprite(const std::vector<SpriteGroup *> &sprite_groups);
    virtual ~SimpleSprite() = default;

    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime);
    SpriteType type{SIMPLESPRITE};
    Rectangle rect{};

protected:

    TiledTexture image{};

private:

    std::vector<SpriteGroup *> groups;
};

class Sprite : public SimpleSprite
{
public:

    Sprite(Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs, int z_ = WORLD_LAYERS["main"]);

    int z = WORLD_LAYERS["main"];
    int y_sort{};
    Rectangle hitbox{};

private:

    std::vector<SpriteGroup *> groups;
};

// It is a Sprite with a small offset in the drawing sorting (delay in draw)
class MonsterPatchSprite : public Sprite
{
public:

    MonsterPatchSprite(Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs, std::string bio);

private:

    std::string biome;
};

class BorderSprite : public Sprite
{
public:

    BorderSprite(Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs);

    Rectangle hitbox;
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(
            Vector2 position, const std::vector<TiledTexture> &frms, const std::vector<SpriteGroup *> &sgs,
            int z = WORLD_LAYERS["main"]);
    void Animate(double deltaTime);
    void Update(double deltaTime) override;

private:

    double frame_index;
    std::vector<TiledTexture> frames;
};

int GetZ(const SimpleSprite *sprite);
int GetYsort(const SimpleSprite *sprite);

class SpriteGroup
{
public:

    virtual ~SpriteGroup() = default;
    virtual void Draw() const;
    void Update(double deltaTime);
    std::vector<SimpleSprite *> sprites;
};
