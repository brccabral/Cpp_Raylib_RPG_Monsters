#pragma once
#include <raylib.h>
#include <vector>

class SpriteGroup;
class Sprite
{
public:

    Sprite(Vector2 position, Texture2D *image, SpriteGroup *sprite_group, Rectangle imgRect);
    virtual ~Sprite();
    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime);
    int SpriteType{0};

protected:

    Vector2 position{};
    Rectangle imgRect{};
    Texture2D *image;

private:

    std::vector<SpriteGroup *> groups;
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(Vector2 position, std::vector<Texture2D> &frames, SpriteGroup *sprite_group, Rectangle imgRect);
};

class SpriteGroup
{
public:

    virtual ~SpriteGroup() = default;
    void Draw() const;
    void Update(double deltaTime);
    std::vector<Sprite *> sprites;
};
