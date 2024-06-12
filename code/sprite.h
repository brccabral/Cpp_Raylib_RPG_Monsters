#pragma once
#include <raylib.h>
#include <vector>

class SpriteGroup;

class SimpleSprite
{
public:


    explicit SimpleSprite(SpriteGroup *sprite_group);
    virtual ~SimpleSprite();
    virtual void Draw(Vector2 offset) const;
    virtual void Update(double deltaTime);
    int SpriteType{0};

protected:

    Vector2 position{};
    Rectangle imgRect{};
    Texture2D *image = nullptr;

private:

    std::vector<SpriteGroup *> groups;
};

class Sprite : public SimpleSprite
{
public:

    Sprite(Vector2 pos, Texture2D *im, SpriteGroup *sg, Rectangle rect);

private:

    std::vector<SpriteGroup *> groups;
};

class AnimatedSprite : public Sprite
{
public:

    AnimatedSprite(Vector2 position, std::vector<Texture2D> &frames, SpriteGroup *sprite_group, Rectangle imgRect);
    void Animate(double deltaTime);
    void Update(double deltaTime) override;

private:

    double frame_index;
    std::vector<Texture2D> frames;
};

class SpriteGroup
{
public:

    virtual ~SpriteGroup() = default;
    void Draw() const;
    void Update(double deltaTime);
    std::vector<SimpleSprite *> sprites;
};
