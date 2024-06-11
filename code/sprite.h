#pragma once
#include <raylib.h>
#include <vector>

class SpriteGroup;
class Sprite
{
public:

    Sprite(Vector2 position, Texture2D *image, SpriteGroup *sprite_group, Rectangle imgRect);
    void Draw() const;

private:

    Vector2 position{};
    Texture2D *image;
    Rectangle imgRect{};
    std::vector<SpriteGroup *> groups;
};

class SpriteGroup
{
public:

    void Draw() const;
    void Update();
    std::vector<Sprite *> sprites;
};
