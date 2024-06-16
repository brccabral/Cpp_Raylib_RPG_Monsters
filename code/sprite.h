#pragma once
#include <array>
#include <vector>
#include <raylib.h>
#include "settings.h"


typedef struct TiledTexture
{
    Texture2D *texture; // the atlas image
    Rectangle rect; // this rect is the rect inside the atlas, not the position of an obj on the map
} TiledTexture;

class SpriteGroup;

class SimpleSprite
{
public:


    explicit SimpleSprite(const std::vector<SpriteGroup *> &sprite_groups);
    virtual ~SimpleSprite() = default;

    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime){};
    void LeaveOtherGroups(const SpriteGroup *sprite_group);

    SpriteType type{SIMPLESPRITE};
    Rectangle rect{};

protected:

    TiledTexture image{};
    std::vector<SpriteGroup *> groups;
};

class Sprite : public SimpleSprite
{
public:

    Sprite(Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs, int z_ = WORLD_LAYERS["main"]);

    int z = WORLD_LAYERS["main"];
    int y_sort{};
    Rectangle hitbox{};
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
};

// Locations that changes the map
class TransitionSprite : public Sprite
{
public:

    TransitionSprite(
            Vector2 pos, Vector2 size, std::array<std::string, 2> target, const std::vector<SpriteGroup *> &sgs);

    std::array<std::string, 2> target;
};

class CollidableSprite : public Sprite
{
public:

    CollidableSprite(Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs);
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
Rectangle GetHitbox(const SimpleSprite *sprite);

class SpriteGroup
{
public:

    virtual ~SpriteGroup();
    virtual void Draw() const;
    void Update(double deltaTime);
    std::vector<SimpleSprite *> sprites;
};
