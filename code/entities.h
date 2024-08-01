#pragma once
#include <rygame.h>


class Player : public rg::sprite::Sprite
{
public:

    Player(rg::math::Vector2 pos, const std::vector<rg::sprite::Group *> &groups);
};
