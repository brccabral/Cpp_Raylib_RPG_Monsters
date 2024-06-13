#pragma once
#include "sprite.h"

class Entity : public SimpleSprite
{
public:

    Entity(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_frms,
           const std::vector<SpriteGroup *> &sgs, std::string facing_dir);
    void Animate(double dt);
    void Update(double dt) override;

    int z = WORLD_LAYERS["main"];
    int y_sort{};
    Rectangle hitbox{};
    std::string facing_direction = "down";

protected:

    Vector2 direction{};
    float speed = 250;

private:

    std::string GetState();

    double frame_index{};
    std::map<std::string, std::vector<TiledTexture>> named_frames{};
};

class Character : public Entity
{
public:

    Character(
            Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_frms,
            const std::vector<SpriteGroup *> &sgs, std::string facing_dir);
};

class Player : public Entity
{
public:

    Player(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_frms,
           const std::vector<SpriteGroup *> &sgs, std::string facing_dir, SpriteGroup *cs);
    void Input();
    void Move(double deltaTime);
    void Update(double deltaTime) override;
    void Collisions(Axis axis);
    [[nodiscard]] Vector2 GetCenter() const;
    SpriteGroup *collision_sprites;
};
