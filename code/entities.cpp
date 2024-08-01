#include "entities.h"

Player::Player(const rg::math::Vector2 pos, const std::vector<rg::sprite::Group *> &groups)
    : Sprite(groups)
{
    image = new rg::Surface(100, 100);
    image->Fill(rl::RED);
    rect = image->GetRect();
    rect.center(pos);
}
