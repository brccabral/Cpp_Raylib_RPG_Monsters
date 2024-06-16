#pragma once
#include "game_data.h"
#include "sprite.h"
#include "timer.h"

class Game;
class Entity : public SimpleSprite
{
public:

    Entity(Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
           const std::vector<SpriteGroup *> &sgs, FacingDirection facing_dir);
    void Animate(double dt);
    void Update(double dt) override;
    void Block();
    void Unblock();
    void ChangeFacingDirection(Vector2 target_pos);

    int z = WORLD_LAYERS["main"];
    int y_sort{};
    Rectangle hitbox{};
    FacingDirection facing_direction = DOWN;
    bool blocked{}; // block player movements

protected:

    Vector2 direction{};
    float speed = 250;

private:

    FacingDirection GetState();

    double frame_index{};
    std::map<FacingDirection, std::vector<TiledTexture>> face_frames{};
};

class Player : public Entity
{
public:

    Player(Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
           const std::vector<SpriteGroup *> &sgs, FacingDirection facing_dir, SpriteGroup *cs);
    void Update(double deltaTime) override;
    [[nodiscard]] Vector2 GetCenter() const;

    bool noticed = false;

private:

    void Input();
    void Move(double deltaTime);
    void Collisions(Axis axis);
    SpriteGroup *collision_sprites;
};


class Character : public Entity
{
public:

    Character(
            Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
            const std::vector<SpriteGroup *> &sgs, FacingDirection facing_dir, CharacterData char_data, float radius,
            Game *g);
    ~Character() override;
    [[nodiscard]] std::vector<std::string> GetDialog() const;
    void Update(double dt) override;
    void StartMove();
    void Move(double dt);
    void RandomViewDirection();

    bool can_rotate = true;

private:

    void Raycast();
    bool HasLineOfSight();

    CharacterData character_data;
    std::vector<Rectangle> collition_rects;
    bool has_moved{};
    bool has_noticed{};
    float radius{};
    std::vector<FacingDirection> view_directions = {LEFT, RIGHT};
    Game *game;
    std::map<std::string, Timer *> timers;
};
