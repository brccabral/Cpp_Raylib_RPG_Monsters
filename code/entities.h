#pragma once
#include "sprite.h"
#include "support.h"

class Entity : public SimpleSprite
{
public:

    Entity(Vector2 pos, tilemap_name fr, SpriteGroup *sg, Rectangle rect);

private:

    double frame_index;
    tilemap_name frames;
};

class Player : public Entity
{
public:

    Player(Vector2 position, const tilemap_name &frames, SpriteGroup *sprite_group, Rectangle imgRect);
    // void Draw(Vector2 offset) const;
    void Input();
    void Move(double deltaTime);
    void Update(double deltaTime) override;
    [[nodiscard]] Vector2 GetCenter() const;
    Vector2 direction{};
};
