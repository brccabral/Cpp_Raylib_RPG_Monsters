#pragma once
#include "sprite.h"
#include "support.h"

class Entity : public SimpleSprite
{
public:

    Entity(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg);
    void Animate(double dt);
    void Update(double dt) override;

protected:

    Vector2 direction{};
    float speed = 250;
    std::string facing_direction = "down";

private:

    std::string GetState();

    double frame_index;
    std::map<std::string, std::vector<TiledTexture>> frames;
};

class Character : public Entity
{
public:

    Character(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg);
};

class Player : public Entity
{
public:

    Player(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg);
    void Input();
    void Move(double deltaTime);
    void Update(double deltaTime) override;
    [[nodiscard]] Vector2 GetCenter() const;
};
