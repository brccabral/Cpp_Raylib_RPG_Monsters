#pragma once
#include "sprite.h"
#include "support.h"

class Entity : public SimpleSprite
{
public:

    Entity(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg);

private:

    double frame_index;
    std::map<std::string, std::vector<TiledTexture>> frames;
};

class Player : public Entity
{
public:

    Player(Vector2 pos, const std::map<std::string, std::vector<TiledTexture>> &named_tts, SpriteGroup *sg);
    void Input();
    void Move(double deltaTime);
    void Update(double deltaTime) override;
    [[nodiscard]] Vector2 GetCenter() const;
    Vector2 direction{};
};
