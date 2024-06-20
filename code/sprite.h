#pragma once


#include <array>
#include <vector>
#include <raylib.h>
#include "raylib_utils.h"
#include "settings.h"
#include "monster.h"


typedef struct TiledTexture
{
    Texture2D *texture; // the atlas image
    RectangleU
            rect; // this rect is the rect inside the atlas, not the position of an obj on the map
} TiledTexture;

class SimpleSprite;

class SpriteGroup
{
public:

    virtual ~SpriteGroup();
    virtual void Draw() const;
    void Update(double deltaTime);
    std::vector<SimpleSprite *> sprites;
};

class SimpleSprite
{
public:

    explicit SimpleSprite(const std::vector<SpriteGroup *> &sprite_groups);
    virtual ~SimpleSprite();

    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime){};
    void LeaveOtherGroups(const SpriteGroup *sprite_group);
    virtual void FlipH();

    SpriteType type{SIMPLESPRITE};
    RectangleU rect{}; // world position

protected:

    TiledTexture image{}; // contains texture atlas, and atlas position
    std::vector<SpriteGroup *> groups;
};

int GetZ(const SimpleSprite *sprite);
int GetYsort(const SimpleSprite *sprite);
RectangleU GetHitbox(const SimpleSprite *sprite);

// Overworld sprites
class Sprite : public SimpleSprite
{
public:

    Sprite(Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs,
           int z_ = WORLD_LAYERS["main"]);

    int z = WORLD_LAYERS["main"];
    int y_sort{};
    RectangleU hitbox{};
};

// It is a Sprite with a small offset in the drawing sorting (delay in draw)
class MonsterPatchSprite : public Sprite
{
public:

    MonsterPatchSprite(
            Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs,
            std::string bio);

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
            Vector2 pos, Vector2 size, std::array<std::string, 2> target,
            const std::vector<SpriteGroup *> &sgs);

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
            Vector2 position, const std::vector<TiledTexture> &frms,
            const std::vector<SpriteGroup *> &sgs, int z = WORLD_LAYERS["main"]);
    void Animate(double deltaTime);
    void Update(double deltaTime) override;
    void FlipH() override;

private:

    double frame_index;
    std::vector<TiledTexture> frames;
};

// Battle Sprites
class MonsterSprite : public SimpleSprite
{
public:

    MonsterSprite(
            Vector2 position, const std::map<std::string, std::vector<TiledTexture>> &frms,
            const std::vector<SpriteGroup *> &sgs, Monster monster, int index, int pos_index,
            const std::string &entity);
    void Animate(double dt);
    void Update(double dt) override;
    void FlipH() override;
    Monster monster;

private:

    int index;
    int pos_index;
    std::string entity;

    float frame_index{};
    std::map<std::string, std::vector<TiledTexture>> frames;
    std::string state = "idle";
    float animation_speed{};
};

class MonsterNameSprite : public SimpleSprite
{
public:

    MonsterNameSprite(
            Vector2 pos, MonsterSprite *monster_sprite, const std::vector<SpriteGroup *> &sgs,
            Font font);
    ~MonsterNameSprite() override;

private:

    MonsterSprite *monster_sprite;
    RenderTexture2D render{};
};

class MonsterLevelSprite : public SimpleSprite
{
public:

    MonsterLevelSprite(
            const std::string &entity, Vector2 pos, MonsterSprite *monster_sprite,
            const std::vector<SpriteGroup *> &sgs, const Font &font);
    ~MonsterLevelSprite() override;
    void Update(double deltaTime) override;

private:

    void UpdateTexture() const;

    RenderTexture2D render{};
    RenderTexture2D inverted{}; // as we update everyframe, inverted is a class member
    std::string entity;
    MonsterSprite *monster_sprite;
    Font font;
    RectangleU xp_rect{};
};

class MonsterStatsSprite : public SimpleSprite
{
public:

    MonsterStatsSprite(
            Vector2 pos, MonsterSprite *monster_sprite, Vector2 size,
            const std::vector<SpriteGroup *> &sgs, Font font);
    ~MonsterStatsSprite() override;
    void Update(double deltaTime) override;

private:

    MonsterSprite *monster_sprite;
    RenderTexture2D render{};
    RenderTexture2D inverted{};
    Font font;
};
