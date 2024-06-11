#pragma once
#include <raylib.h>
#include <sched.h>
#include <vector>

class SpriteGroup;
class Sprite
{
public:

    Sprite(Vector2 position, Texture2D *image, SpriteGroup *sprite_group, Rectangle imgRect);
    virtual ~Sprite();
    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime);

protected:

    Vector2 position{};
    Rectangle imgRect{};

private:

    Texture2D *image;
    std::vector<SpriteGroup *> groups;
};

class SpriteGroup
{
public:

    virtual ~SpriteGroup(){};
    void Draw() const;
    void Update(double deltaTime);
    std::vector<Sprite *> sprites;
};
