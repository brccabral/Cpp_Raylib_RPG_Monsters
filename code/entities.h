#pragma once
#include "game_data.h"
#include "sprite.h"

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

protected:

    Vector2 direction{};
    float speed = 250;
    bool blocked{}; // block player movements

private:

    FacingDirection GetState();

    double frame_index{};
    std::map<FacingDirection, std::vector<TiledTexture>> face_frames{};
};

class Character : public Entity
{
public:

    Character(
            Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
            const std::vector<SpriteGroup *> &sgs, FacingDirection facing_dir, const CharacterData &character_data);
};

class Player : public Entity
{
public:

    Player(Vector2 pos, const std::map<FacingDirection, std::vector<TiledTexture>> &face_frms,
           const std::vector<SpriteGroup *> &sgs, FacingDirection facing_dir, SpriteGroup *cs);
    void Input();
    void Move(double deltaTime);
    void Update(double deltaTime) override;
    void Collisions(Axis axis);
    [[nodiscard]] Vector2 GetCenter() const;
    SpriteGroup *collision_sprites;
};
