#include "battle.hpp"

#include "sprite.hpp"


Battle::Battle(
        std::map<int, Monster> *player_monsters, std::map<int, Monster> *opponent_monsters,
        std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>> *monster_frames,
        const std::shared_ptr<rg::Surface> &bg_surf,
        std::map<std::string, std::shared_ptr<rg::font::Font>> *fonts)
    : player_monsters(player_monsters), opponent_monsters(opponent_monsters),
      monster_frames(monster_frames), bg_surf(bg_surf), fonts(fonts)
{
    Setup();
}

void Battle::Update(float dt)
{
    int a = player_monsters->size();
    int b = opponent_monsters->size();
    int c = monster_frames->size();
    int d = bg_surf->GetRect().width;
    int e = fonts->size();
    int p = a + b + c + d + e;
    display_surface->Blit(bg_surf, {float(p - p), 0});
    battle_sprites.Draw(display_surface);
}

void Battle::Setup()
{
    for (auto &[index, monster]: *player_monsters)
    {
        if (index <= 2)
        {
            CreateMonster(&monster, index, index, "player");
        }
    }
}

void Battle::CreateMonster(Monster *monster, int index, int pos_index, const std::string &entity)
{
    auto frames = (*monster_frames)[monster->name];
    auto pos = rg::math::Vector2{0, 0};
    auto groups = std::vector<rg::sprite::Group *>{};

    if (!std::strcmp(entity.c_str(), "player"))
    {
        pos = BATTLE_POSITIONS["left"][pos_index];
        groups = {&battle_sprites, &player_sprites};
        std::make_shared<MonsterSprite>(pos, frames, monster, index, pos_index, entity)
                ->add(groups);
    }
}
