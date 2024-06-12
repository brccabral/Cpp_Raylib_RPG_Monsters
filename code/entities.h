#pragma once
#include "sprite.h"
#include "support.h"

class Entity : public SimpleSprite
{
public:

    Entity(Vector2 pos, Texture2D *tx, SpriteGroup *sg);

private:

    double frame_index;
    tilerect_name frames;
};

class Player : public Entity
{
public:

    Player(Vector2 position, Texture2D *tx, SpriteGroup *sprite_group, tilerect_name rects);
    void Input();
    void Move(double deltaTime);
    void Update(double deltaTime) override;
    [[nodiscard]] Vector2 GetCenter() const;
    Vector2 direction{};
};
