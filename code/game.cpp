#include <memory>
#include "game.hpp"
#include "settings.hpp"
#include "sprite.hpp"
#include "support.hpp"
#include "game_data.hpp"


Game::Game()
{
    rg::Init(rl::LOG_WARNING);
    display_surface = rg::display::SetMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    rg::display::SetCaption("RPG Monsters");

    int player_index = 0;
    player_monsters[player_index++] = std::make_shared<Monster>("Charmadillo", 2);
    player_monsters[player_index++] = std::make_shared<Monster>("Friolera", 2);
    player_monsters[player_index++] = std::make_shared<Monster>("Larvea", 2);
    player_monsters[player_index++] = std::make_shared<Monster>("Atrox", 2);
    // player_monsters[player_index++] = std::make_shared<Monster>("Sparchu", 24);
    // player_monsters[player_index++] = std::make_shared<Monster>("Gulfin", 24);
    // player_monsters[player_index++] = std::make_shared<Monster>("Jacana", 2);
    // player_monsters[player_index++] = std::make_shared<Monster>("Pouch", 3);
    // player_monsters[player_index++] = std::make_shared<Monster>("Finsta", 30);

    // for (auto &[i, monster]: player_monsters)
    // {
    //     monster->health *= 0.5f;
    //     monster->energy *= 0.5f;
    // }

    // int dummy_index = 0;
    // dummy_monsters[dummy_index++] = std::make_shared<Monster>("Atrox", 100);
    // dummy_monsters[dummy_index++] = std::make_shared<Monster>("Sparchu", 3);
    // dummy_monsters[dummy_index++] = std::make_shared<Monster>("Gulfin", 3);
    // dummy_monsters[dummy_index++] = std::make_shared<Monster>("Jacana", 2);
    // dummy_monsters[dummy_index++] = std::make_shared<Monster>("Pouch", 3);

    all_sprites = std::make_shared<AllSprites>();

    // transition / tint
    transition_target = {};
    tint_surf = std::make_shared<rg::Surface>(WINDOW_WIDTH, WINDOW_HEIGHT);

    ImportAssets();
    Setup("world", "house");

    monster_index = std::make_shared<MonsterIndex>(
            &player_monsters, fonts, &monster_icons, &ui_icons, &monster_frames);

    encounter_timer = rg::Timer(2.0f, false, false, [this] { MonsterEncounter(); });

    CheckEvolution();
}

Game::~Game()
{
    UnloadResources();
}

void Game::run()
{
    while (!rg::WindowCloseOrQuit())
    {
        const float dt = rg::time::Clock::tick();
        display_surface->Fill(rl::BLACK);

        encounter_timer.Update();
        // game logic
        Input();
        TransitionCheck();
        if (!battle)
        {
            all_sprites->Update(dt);
            all_sprites->Draw(player);
        }
        CheckMonster();

        // overlays
        if (dialog_tree)
        {
            dialog_tree->Update();
        }
        if (index_open)
        {
            monster_index->Update(dt);
        }
        if (battle)
        {
            battle->Update(dt);
        }
        if (evolution)
        {
            evolution->Update(dt);
        }

        TintScreen(dt);
        rg::display::Update();
    }
}

void Game::ImportAssets()
{
    tmx_maps = rg::tmx::LoadTMXMaps("resources/data/maps");

    const auto waterList = rg::image::LoadFolderList("resources/graphics/tilesets/water");
    waterFrames = rg::Frames::Merge(waterList, 1, waterList.size());

    cost_dict = CoastImporter("resources/graphics/tilesets/coast.png", 12, 24);
    characters_dict = AllCharacterImport("resources/graphics/characters");

    fonts["dialog"] =
            std::make_shared<rg::font::Font>("resources/graphics/fonts/PixeloidSans.ttf", 30);
    fonts["regular"] =
            std::make_shared<rg::font::Font>("resources/graphics/fonts/PixeloidSans.ttf", 18);
    fonts["small"] =
            std::make_shared<rg::font::Font>("resources/graphics/fonts/PixeloidSans.ttf", 14);
    fonts["bold"] =
            std::make_shared<rg::font::Font>("resources/graphics/fonts/dogicapixelbold.otf", 20);

    monster_icons = rg::image::ImportFolderDict("resources/graphics/icons");
    ui_icons = rg::image::ImportFolderDict("resources/graphics/ui");

    bg_frames = rg::image::ImportFolderDict("resources/graphics/backgrounds");

    monster_frames = MonsterImporter(4, 2, "resources/graphics/monsters");
    outline_frames = OutlineCreator(monster_frames, 4);

    attack_frames = AttackImporter("resources/graphics/attacks");
}

void Game::Setup(const std::string &map_name, const std::string &player_start_position)
{
    // clear the map
    all_sprites->empty();
    collision_sprites.empty();
    character_sprites.empty();
    transition_sprites.empty();

    const rl::tmx_map *map = tmx_maps[map_name];

    const rl::tmx_layer *terrain_layer = tmx_find_layer_by_name(map, "Terrain");
    const rl::tmx_layer *terrain_top_layer = tmx_find_layer_by_name(map, "Terrain Top");
    const rl::tmx_layer *water_layer = tmx_find_layer_by_name(map, "Water");
    const rl::tmx_layer *coast_layer = tmx_find_layer_by_name(map, "Coast");
    const rl::tmx_layer *objects_layer = tmx_find_layer_by_name(map, "Objects");
    const rl::tmx_layer *transition_layer = tmx_find_layer_by_name(map, "Transition");
    const rl::tmx_layer *collision_layer = tmx_find_layer_by_name(map, "Collisions");
    const rl::tmx_layer *monster_layer = tmx_find_layer_by_name(map, "Monsters");
    const rl::tmx_layer *entities_layer = tmx_find_layer_by_name(map, "Entities");

    // terrain
#if 0
    auto terrain_tiles = rg::tmx::GetTMXTiles(map, terrain_layer);
    for (auto &[position, texture, atlas_rect]: terrain_tiles)
    {
        auto surface = std::make_shared<rg::Surface>(texture, atlas_rect);
        std::make_shared<Sprite>(position, surface, WORLD_LAYERS["bg"])->add(all_sprites.get());
    }

    auto terrain_top_tiles = rg::tmx::GetTMXTiles(map, terrain_top_layer);
    for (auto &[position, texture, atlas_rect]: terrain_top_tiles)
    {
        auto surface = std::make_shared<rg::Surface>(texture, atlas_rect);
        std::make_shared<Sprite>(position, surface, WORLD_LAYERS["bg"])->add(all_sprites.get());
    }
#else
    const auto terrain_surf = rg::tmx::GetTMXLayerSurface(map, terrain_layer);
    std::make_shared<Sprite>(rg::math::Vector2{}, terrain_surf, WORLD_LAYERS["bg"])
            ->add(all_sprites.get());

    const auto terrain_top_surf = rg::tmx::GetTMXLayerSurface(map, terrain_top_layer);
    std::make_shared<Sprite>(rg::math::Vector2{}, terrain_top_surf, WORLD_LAYERS["bg"])
            ->add(all_sprites.get());
#endif

    // water
    auto water = water_layer->content.objgr->head;
    while (water)
    {
        auto area_position = rg::tmx::GetTMXObjPosition(water);
        for (int y = 0; y < water->height; y += TILE_SIZE)
        {
            for (int x = 0; x < water->width; x += TILE_SIZE)
            {
                auto position = area_position + rg::math::Vector2{(float) x, (float) y};
                std::make_shared<AnimatedSprite>(position, waterFrames, WORLD_LAYERS["water"])
                        ->add(all_sprites.get());
            }
        }
        water = water->next;
    }

    // coast
    auto coast = coast_layer->content.objgr->head;
    while (coast)
    {
        const char *terrain = rl::tmx_get_property(coast->properties, "terrain")->value.string;
        const char *side = rl::tmx_get_property(coast->properties, "side")->value.string;

        auto position = rg::tmx::GetTMXObjPosition(coast);
        auto t = cost_dict[terrain];
        auto s = t[side];
        std::make_shared<AnimatedSprite>(position, s, WORLD_LAYERS["bg"])->add(all_sprites.get());

        coast = coast->next;
    }

    // objects
    auto object = objects_layer->content.objgr->head;
    while (object)
    {
        const int gid = object->content.gid;
        if (map->tiles[gid])
        {
            rg::Rect atlas_rect;
            auto *tileTexture = rg::tmx::GetTMXTileTexture(map->tiles[gid], &atlas_rect);
            auto objSurf = std::make_shared<rg::Surface>(tileTexture, atlas_rect);
            auto position = rg::tmx::GetTMXObjPosition(object);

            std::string name{};
            if (object->name)
            {
                name = object->name;
            }

            if (!strcmp(name.c_str(), "top"))
            {
                std::make_shared<Sprite>(position, objSurf, WORLD_LAYERS["top"])
                        ->add(all_sprites.get());
            }
            else
            {
                std::make_shared<CollidableSprite>(position, objSurf)
                        ->add({all_sprites.get(), &collision_sprites});
            }
        }
        object = object->next;
    }

    // transition objects
    auto transition = transition_layer->content.objgr->head;
    while (transition)
    {
        std::string target = rl::tmx_get_property(transition->properties, "target")->value.string;
        std::string pos = rl::tmx_get_property(transition->properties, "pos")->value.string;
        std::make_shared<TransitionSprite>(
                rg::math::Vector2{float(transition->x), float(transition->y)},
                rg::math::Vector2{float(transition->width), float(transition->height)},
                std::make_pair(target, pos))
                ->add(&transition_sprites);
        transition = transition->next;
    }

    // collision objects
    auto collision = collision_layer->content.objgr->head;
    while (collision)
    {
        auto position = rg::tmx::GetTMXObjPosition(collision);
        std::make_shared<BorderSprite>(
                position, std::make_shared<rg::Surface>(collision->width, collision->height))
                ->add(&collision_sprites);
        collision = collision->next;
    }

    // monster patches
    auto monster = monster_layer->content.objgr->head;
    while (monster)
    {
        const int gid = monster->content.gid;
        if (map->tiles[gid])
        {
            std::string biome = rl::tmx_get_property(monster->properties, "biome")->value.string;
            std::string monsters =
                    rl::tmx_get_property(monster->properties, "monsters")->value.string;
            int level = rl::tmx_get_property(monster->properties, "level")->value.integer;
            rg::Rect atlas;
            auto monster_texture = rg::tmx::GetTMXTileTexture(map->tiles[gid], &atlas);
            auto position = rg::tmx::GetTMXObjPosition(monster);
            auto monster_surf = std::make_shared<rg::Surface>(monster_texture, atlas);
            std::make_shared<MonsterPatchSprite>(position, monster_surf, biome, monsters, level)
                    ->add({all_sprites.get(), &monster_sprites});
        }
        monster = monster->next;
    }

    // we need to find the player first, so it can be passed to all characters in the next look
    auto entity = entities_layer->content.objgr->head;
    while (entity)
    {
        auto position = rg::tmx::GetTMXObjPosition(entity);
        const char *direction = rl::tmx_get_property(entity->properties, "direction")->value.string;
        if (std::strcmp(entity->name, "Player") == 0)
        {
            const char *entity_pos = rl::tmx_get_property(entity->properties, "pos")->value.string;
            if (std::strcmp(entity_pos, player_start_position.c_str()) == 0)
            {
                player = std::make_shared<Player>(
                        position, characters_dict["player"], direction, &collision_sprites);
                player->add(all_sprites.get());
            }
        }
        entity = entity->next;
    }

    // we can only create Characters after we have Player
    entity = entities_layer->content.objgr->head;
    while (entity)
    {
        auto position = rg::tmx::GetTMXObjPosition(entity);
        const char *direction = rl::tmx_get_property(entity->properties, "direction")->value.string;
        if (std::strcmp(entity->name, "Character") == 0)
        {
            std::string character_id =
                    rl::tmx_get_property(entity->properties, "character_id")->value.string;
            const char *graphic = rl::tmx_get_property(entity->properties, "graphic")->value.string;
            int radius =
                    std::stoi(rl::tmx_get_property(entity->properties, "radius")->value.string);
            bool nurse = std::strcmp(character_id.c_str(), "Nurse") == 0;
            std::make_shared<Character>(
                    position, characters_dict[graphic], direction, &TRAINER_DATA[character_id],
                    player, [this](const std::shared_ptr<Character> &char_)
                    { CreateDialog(char_); }, &collision_sprites, radius, nurse)
                    ->add({all_sprites.get(), &collision_sprites, &character_sprites});
        }
        entity = entity->next;
    }
}

void Game::UnloadResources()
{
    for (auto &[key, tmx_map]: tmx_maps)
    {
        UnloadTMX(tmx_map);
    }
}

void Game::Input()
{
    if (dialog_tree)
    {
        return;
    }
    if (battle)
    {
        return;
    }
    if (IsKeyPressed(rl::KEY_SPACE))
    {
        for (const auto &character_sprite: character_sprites.Sprites())
        {
            auto character = std::dynamic_pointer_cast<Character>(character_sprite);
            if (CheckConnections(100, player, character))
            {
                player->Block();
                character->ChangeFacingDirection(player->rect.center());
                CreateDialog(character);
                character->can_rotate = false;
            }
        }
    }
    if (IsKeyPressed(rl::KEY_ENTER))
    {
        index_open = !index_open;
        player->blocked = !player->blocked;
    }
}

void Game::CreateDialog(const std::shared_ptr<Character> &character)
{
    if (!dialog_tree)
    {
        dialog_tree = std::make_shared<DialogTree>(
                character, player, all_sprites.get(), fonts["dialog"],
                [this](const std::shared_ptr<Character> &char_) { EndDialog(char_); });
    }
}

void Game::EndDialog(const std::shared_ptr<Character> &character)
{
    dialog_tree = nullptr;
    if (character->nurse)
    {
        for (auto &[i, monster]: player_monsters)
        {
            monster->health = monster->GetStat("max_health");
            monster->energy = monster->GetStat("max_energy");
        }
        player->Unblock();
    }
    else if (!character->character_data->defeated)
    {
        if (transition_target)
        {
            transition_target.reset();
            transition_target = nullptr;
        }
        transition_target = std::make_shared<TransitionTarget>(TRANSITIONTARGET_LEVEL2BATTLE);
        transition_target->battle = std::make_shared<Battle>(
                &player_monsters, &character->monsters, &monster_frames, &outline_frames,
                &monster_icons, &ui_icons, &attack_frames,
                bg_frames[character->character_data->biome], &fonts,
                [this](const std::shared_ptr<Character> &c) { EndBattle(c); }, character);
        tint_mode = TINT;
    }
    else
    {
        player->Unblock();
    }
}

void Game::TransitionCheck()
{
    if (transition_target)
    {
        return;
    }
    std::vector<std::shared_ptr<TransitionSprite>> sprites;
    // transition_sprites must contain only TransitionSprite
    for (const auto &transition_sprite: transition_sprites.Sprites())
    {
        if (transition_sprite->rect.colliderect(player->hitbox))
        {
            sprites.push_back(std::dynamic_pointer_cast<TransitionSprite>(transition_sprite));
        }
    }
    if (!sprites.empty())
    {
        player->Block();
        transition_target = std::make_shared<TransitionTarget>(TRANSITIONTARGET_MAP);
        transition_target->map_name = sprites[0]->target.first;
        transition_target->start_position = sprites[0]->target.second;
        tint_mode = TINT;
    }
}

void Game::TintScreen(const double dt)
{
    if (tint_mode == UNTINT)
    {
        tint_progress -= tint_speed * dt;
    }
    if (tint_mode == TINT)
    {
        tint_progress += tint_speed * dt;
        if (tint_progress >= 255)
        {
            if (transition_target->target_type == TRANSITIONTARGET_LEVEL2BATTLE)
            {
                battle = transition_target->battle;
            }
            else if (transition_target->target_type == TRANSITIONTARGET_BATTLE2LEVEL)
            {
                battle.reset();
                battle = nullptr;
            }
            else if (transition_target->target_type == TRANSITIONTARGET_MAP)
            {
                Setup(transition_target->map_name, transition_target->start_position);
            }
            tint_mode = UNTINT;
            transition_target.reset();
            transition_target = nullptr;
        }
    }
    tint_progress = rl::Clamp(tint_progress, 0, 255);
    tint_surf->SetAlpha(tint_progress);
    display_surface->Blit(tint_surf, rg::math::Vector2{0, 0});
}

void Game::EndBattle(const std::shared_ptr<Character> &character)
{
    if (transition_target)
    {
        transition_target.reset();
        transition_target = nullptr;
    }
    transition_target = std::make_shared<TransitionTarget>(TRANSITIONTARGET_BATTLE2LEVEL);
    tint_mode = TINT;
    if (character)
    {
        character->character_data->defeated = true;
        CreateDialog(character);
    }
    else if (!evolution)
    {
        player->Unblock();
        CheckEvolution();
    }
}

void Game::CheckMonster()
{
    if (battle || !player->direction || encounter_timer.active)
    {
        return;
    }
    for (const auto &sprite: monster_sprites.Sprites())
    {
        if (sprite->rect.colliderect(player->hitbox))
        {
            encounter_timer.Activate();
        }
    }
}

void Game::MonsterEncounter()
{
    if (!player->direction)
    {
        return;
    }
    for (const auto &sprite: monster_sprites.Sprites())
    {
        if (sprite->rect.colliderect(player->hitbox))
        {
            // change encounter timer duration for the next encounter
            encounter_timer.duration = rl::GetRandomValue(8, 25) / 10.0f;

            const auto monster_patch_sprite = std::dynamic_pointer_cast<MonsterPatchSprite>(sprite);

            player->Block();
            if (transition_target)
            {
                transition_target.reset();
                transition_target = nullptr;
            }
            transition_target = std::make_shared<TransitionTarget>(TRANSITIONTARGET_LEVEL2BATTLE);

            encounter_monsters.clear();
            int count_monsters = 0;
            for (auto &monster_name: monster_patch_sprite->monsters)
            {
                int level = monster_patch_sprite->level + rl::GetRandomValue(-3, 3);
                encounter_monsters[count_monsters++] =
                        std::make_shared<Monster>(monster_name, level);
            }

            transition_target->battle = std::make_shared<Battle>(
                    &player_monsters, &encounter_monsters, &monster_frames, &outline_frames,
                    &monster_icons, &ui_icons, &attack_frames,
                    bg_frames[monster_patch_sprite->biome], &fonts,
                    [this](const std::shared_ptr<Character> &c) { EndBattle(c); }, nullptr);
            tint_mode = TINT;
            break; // only need the first sprite
        }
    }
}

void Game::CheckEvolution()
{
    for (auto &[index, monster]: player_monsters)
    {
        if (monster->evolution.second) // if evolution level
        {
            if (monster->level >= monster->evolution.second)
            {
                player->Block();
                if (evolution)
                {
                    evolution.reset();
                    evolution = nullptr;
                }
                evolution = std::make_shared<Evolution>(
                        monster_frames, monster->name, monster->evolution.first, fonts["bold"],
                        [this] { EndEvolution(); });
            }
        }
    }
}

void Game::EndEvolution()
{
    evolution.reset();
    evolution = nullptr;
    player->Unblock();
}
