#pragma once
#include <rygame.h>


class Sprite : public rg::sprite::Sprite
{
public:

    Sprite(rg::math::Vector2 pos, rg::Surface *surf,
           const std::vector<rg::sprite::Group *> &groups);
};
