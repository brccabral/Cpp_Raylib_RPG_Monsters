#pragma once
#include <array>
#include <raylib.h>
#include "raylib_utils.h"
#include "game_data.h"
#include "monster.h"
#include "settings.h"
#include "timer.h"


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
    std::vector<SimpleSprite *> to_delete;
};

class SimpleSprite
{
public:

    explicit SimpleSprite(const std::vector<SpriteGroup *> &sprite_groups);
    virtual ~SimpleSprite();

    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime){};
    void LeaveOtherGroups(const SpriteGroup *sprite_group);
    virtual void Kill();
    virtual void FlipH();

    SpriteType type{SIMPLESPRITE};
    RectangleU rect{}; // world position
    TiledTexture image{}; // contains texture atlas, and atlas position
    std::vector<SpriteGroup *> groups;
    bool killed{};
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
    virtual void Animate(double deltaTime);
    void Update(double deltaTime) override;
    void FlipH() override;

protected:

    double frame_index;
    std::vector<TiledTexture> frames;
};

class Battle; // forward declaration
class MonsterNameSprite; // forward declaration
class MonsterLevelSprite; // forward declaration
class MonsterStatsSprite; // forward declaration
class MonsterOutlineSprite; // forward declaration
// Battle Sprites
class MonsterSprite : public SimpleSprite
{
public:

    MonsterSprite(
            Vector2 position, const std::map<AnimationState, std::vector<TiledTexture>> &frms,
            const std::vector<SpriteGroup *> &sgs, Monster *monster, int index, int pos_index,
            SelectionSide entity, Battle *battle);
    ~MonsterSprite() override;
    void Animate(double dt);
    void Update(double dt) override;
    void FlipH() override;
    void SetHighlight(bool value);
    void ActivateAttack(MonsterSprite *monster_sprite, Attack selected_attack);
    void Kill() override;
    void DelayedKill(Monster *monster, int index, int pos_index, SelectionSide side);
    void Destroy();

    void SetNameSprite(MonsterNameSprite *name_sprite);
    void SetLevelSprite(MonsterLevelSprite *level_sprite);
    void SetStatsSprite(MonsterStatsSprite *stats_sprite);
    void SetOutlineSprite(MonsterOutlineSprite *outline_sprite);

    Monster *monster;
    int z = BATTLE_LAYERS["monster"];
    AnimationState state = ANIMATION_IDLE;
    std::map<AnimationState, std::vector<TiledTexture>> state_frames;
    std::map<AnimationState, std::vector<TiledTexture>> state_frames_highlight;
    int adjusted_frame_index{};
    int pos_index{};
    SelectionSide entity;
    int index;

private:


    float animation_speed{};
    float frame_index{};

    bool highlight{};
    std::map<std::string, Timer *> timers;

    MonsterSprite *target_sprite = nullptr;
    Attack current_attack = ATTACK_NONE;
    Battle *battle;

    MonsterNameSprite *name_sprite_ = nullptr;
    MonsterLevelSprite *level_sprite_ = nullptr;
    MonsterStatsSprite *stats_sprite_ = nullptr;
    MonsterOutlineSprite *outline_sprite_ = nullptr;

    Monster *newMonster = nullptr;
    int newIndex = 0;
    int newPosIndex = 0;
    SelectionSide newSide{};
};

class MonsterNameSprite : public SimpleSprite
{
public:

    MonsterNameSprite(
            Vector2 pos, MonsterSprite *monster_sprite, const std::vector<SpriteGroup *> &sgs,
            Font font);
    ~MonsterNameSprite() override;

    int z = BATTLE_LAYERS["name"];

private:

    MonsterSprite *monster_sprite;
    RenderTexture2D render{};
};

class MonsterLevelSprite : public SimpleSprite
{
public:

    MonsterLevelSprite(
            SelectionSide entity, Vector2 pos, MonsterSprite *monster_sprite,
            const std::vector<SpriteGroup *> &sgs, const Font &font);
    ~MonsterLevelSprite() override;
    void Update(double deltaTime) override;

    int z = BATTLE_LAYERS["name"];

private:

    void UpdateTexture() const;

    RenderTexture2D render{};
    RenderTexture2D inverted{}; // as we update everyframe, inverted is a class member
    SelectionSide entity;
    MonsterSprite *monster_sprite;
    Font font;
    RectangleU xp_rect{};
};

class MonsterStatsSprite : public SimpleSprite
{
public:

    MonsterStatsSprite(
            Vector2 pos, MonsterSprite *monster_sprite, Vector2 size,
            const std::vector<SpriteGroup *> &sgs, const Font &font);
    ~MonsterStatsSprite() override;
    void Update(double deltaTime) override;

    int z = BATTLE_LAYERS["overlay"];

private:

    MonsterSprite *monster_sprite;
    RenderTexture2D render{};
    RenderTexture2D inverted{};
    Font font;
};

class MonsterOutlineSprite : public SimpleSprite
{
public:

    MonsterOutlineSprite(
            MonsterSprite *monster_sprite, const std::vector<SpriteGroup *> &sgs,
            const std::map<AnimationState, std::vector<TiledTexture>> &frms);
    void Update(double deltaTime) override;
    void FlipH() override;

    MonsterSprite *monster_sprite;
    int z = BATTLE_LAYERS["outline"];

private:

    std::map<AnimationState, std::vector<TiledTexture>> state_frames;
};

class AttackSprite : public AnimatedSprite
{
public:

    AttackSprite(
            Vector2 position, const std::vector<TiledTexture> &frms,
            const std::vector<SpriteGroup *> &sgs, int z = BATTLE_LAYERS["overlay"]);
    void Animate(double deltaTime) override;
};

class TimedSprite : public Sprite
{
public:

    TimedSprite(
            Vector2 pos, TiledTexture frame, const std::vector<SpriteGroup *> &sgs, float duration);
    ~TimedSprite() override;
    void Update(double deltaTime) override;

private:

    Timer *death_timer = nullptr;
};
