#pragma once
#include "entities.h"
#include "sprite.h"


class AllSprites : public SpriteGroup
{
public:

    AllSprites();
    ~AllSprites() override;

    void Draw(const Player *player);
    Vector2 offset{};

private:

    Texture2D shadow{};
    Texture2D notice{};
};

class BattleSprites : public SpriteGroup
{
public:

    BattleSprites() = default;
    void
    Draw(const MonsterSprite *current_monster_sprite, SelectionSide side, SelectionMode mode,
         int target_index, SpriteGroup *player_sprites, SpriteGroup *opponent_sprites);
};
