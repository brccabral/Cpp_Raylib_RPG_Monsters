#include <memory>
#include "game.hpp"
#include "settings.hpp"
#include "sprite.hpp"
#include "support.hpp"
#include "game_data.hpp"


Game::Game()
{
    rg::Init(rl::LOG_WARNING);
    display_surface = &rg::display::SetMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    rg::display::SetCaption("RPG Monsters");
    all_sprites = new AllSprites();

    int player_index = 0;
    player_monsters[player_index++] = Monster("Charmadillo", 50);
    player_monsters[player_index++] = Monster("Larvea", 3);
    player_monsters[player_index++] = Monster("Plumette", 14);

    // transition / tint
    tint_surf = rg::Surface(WINDOW_WIDTH, WINDOW_HEIGHT);

    ImportAssets();
    Setup("world", "house");

    audio["overworld"].Play();

    monster_index = MonsterIndex(
            &player_monsters, &fonts, &monster_icons, &ui_icons, &monster_frames);

    encounter_timer = rg::Timer(
            2.0f, false, false, [this]
            {
                MonsterEncounter();
            });
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
        if (battle.battle_over)
        {
            all_sprites->Update(dt);
            all_sprites->Draw(&player);
        }
        CheckMonster();

        // overlays
        if (dialog_tree.active)
        {
            dialog_tree.Update();
        }
        if (index_open)
        {
            monster_index.Update(dt);
        }
        if (!battle.battle_over)
        {
            battle.Update(dt);
        }
        if (evolution.IsActive())
        {
            evolution.Update(dt);
            if (!evolution.IsActive())
            {
                CheckEvolution(); // check if there are more evolutions
            }
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
            rg::font::Font("resources/graphics/fonts/PixeloidSans.ttf", 30.0f);
    fonts["regular"] =
            rg::font::Font("resources/graphics/fonts/PixeloidSans.ttf", 18.0f);
    fonts["small"] =
            rg::font::Font("resources/graphics/fonts/PixeloidSans.ttf", 14.0f);
    fonts["bold"] =
            rg::font::Font("resources/graphics/fonts/dogicapixelbold.otf", 20.0f);

    monster_icons = rg::image::ImportFolderDict("resources/graphics/icons");
    ui_icons = rg::image::ImportFolderDict("resources/graphics/ui");
    for (auto &[ui_name, ui_surf]: ui_icons)
    {
        ui_icons[ui_name + "_gray"] = rg::transform::GrayScale(&ui_surf);
    }

    bg_frames = rg::image::ImportFolderDict("resources/graphics/backgrounds");

    monster_frames = MonsterImporter(4, 2, "resources/graphics/monsters");
    outline_frames = OutlineCreator(monster_frames, 4);

    attack_frames = AttackImporter("resources/graphics/attacks");

    const auto star_surfs = rg::image::ImportFolder("resources/graphics/other/star animation");
    for (const auto &star_surf: star_surfs)
    {
        star_animation_surfs.push_back(rg::transform::Scale2x(&star_surf));
    }

    audio = AudioImporter("resources/audio");
}

void Game::Setup(const std::string &map_name, const std::string &player_start_position)
{
    // clear the map
    all_sprites->empty();
    collision_sprites.empty();
    character_sprites.empty();
    transition_sprites.empty();

    const rl::tmx_map *map = tmx_maps[map_name];
    map_tiles_surfaces_ = rg::tmx::GetTMXSurfaces(map);

    surfaces_.clear();
    surfaces_.reserve(map->width * map->height);
    sprites_.clear();
    sprites_.reserve(map->width * map->height);
    animated_sprites_.clear();
    animated_sprites_.reserve(map->width * map->height);
    collidable_sprites_.clear();
    collidable_sprites_.reserve(map->width * map->height);
    transition_sprites_.clear();
    transition_sprites_.reserve(map->width * map->height);
    border_sprites_.clear();
    border_sprites_.reserve(map->width * map->height);
    monster_patch_sprites_.clear();
    monster_patch_sprites_.reserve(map->width * map->height);
    characters_.clear();
    characters_.reserve(map->width * map->height);

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
        auto surface = rg::Surface(texture, atlas_rect);
        Sprite(position, surface, WORLD_LAYERS["bg"])->add(all_sprites);
    }

    auto terrain_top_tiles = rg::tmx::GetTMXTiles(map, terrain_top_layer);
    for (auto &[position, texture, atlas_rect]: terrain_top_tiles)
    {
        auto surface = rg::Surface(texture, atlas_rect);
        Sprite(position, surface, WORLD_LAYERS["bg"])->add(all_sprites);
    }
#else
    surfaces_.emplace_back(rg::tmx::GetTMXLayerSurface(map, terrain_layer));
    sprites_.emplace_back(rg::math::Vector2{}, &surfaces_.back(), WORLD_LAYERS["bg"]);
    sprites_.back().add(all_sprites);

    surfaces_.emplace_back(rg::tmx::GetTMXLayerSurface(map, terrain_top_layer));
    sprites_.emplace_back(rg::math::Vector2{}, &surfaces_.back(), WORLD_LAYERS["bg"]);
    sprites_.back().add(all_sprites);
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
                animated_sprites_.emplace_back(position, &waterFrames, WORLD_LAYERS["water"]);
                animated_sprites_.back().add(all_sprites);
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
        animated_sprites_.emplace_back(position, &cost_dict[terrain][side], WORLD_LAYERS["bg"]);
        animated_sprites_.back().add(all_sprites);

        coast = coast->next;
    }

    // objects
    auto object = objects_layer->content.objgr->head;
    while (object)
    {
        const int gid = object->content.gid;
        if (map->tiles[gid])
        {
            auto position = rg::tmx::GetTMXObjPosition(object);

            std::string name{};
            if (object->name)
            {
                name = object->name;
            }

            if (!strcmp(name.c_str(), "top"))
            {
                sprites_.emplace_back(position, &map_tiles_surfaces_[gid], WORLD_LAYERS["top"]);
                sprites_.back().add(all_sprites);
            }
            else
            {
                collidable_sprites_.emplace_back(position, &map_tiles_surfaces_[gid]);
                collidable_sprites_.back().add({all_sprites, &collision_sprites});
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
        transition_sprites_.emplace_back(
                rg::math::Vector2{float(transition->x), float(transition->y)},
                rg::math::Vector2{float(transition->width), float(transition->height)},
                std::make_pair(target, pos));
        transition_sprites_.back().add(&transition_sprites);
        transition = transition->next;
    }

    // collision objects
    auto collision = collision_layer->content.objgr->head;
    while (collision)
    {
        auto position = rg::tmx::GetTMXObjPosition(collision);
        surfaces_.emplace_back((int) collision->width, (int) collision->height);
        border_sprites_.emplace_back(
                position, &surfaces_.back());
        border_sprites_.back().add(&collision_sprites);
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
            auto position = rg::tmx::GetTMXObjPosition(monster);
            monster_patch_sprites_.emplace_back(
                    position, &map_tiles_surfaces_[gid], biome, monsters, level);
            monster_patch_sprites_.back().add({all_sprites, &monster_sprites});
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
                player = Player(
                        position, &characters_dict["player"], direction, &collision_sprites);
                player.add(all_sprites);
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
            characters_.emplace_back(
                    position, &characters_dict[graphic], direction,
                    &TRAINER_DATA[character_id],
                    &player, [this](Character *char_)
                    {
                        CreateDialog(char_);
                    }, &collision_sprites, float(radius), nurse, &audio["notice"]);
            characters_.back().add({all_sprites, &collision_sprites, &character_sprites});
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
    if (dialog_tree.active)
    {
        return;
    }
    if (!battle.battle_over)
    {
        return;
    }
    if (IsKeyPressed(rl::KEY_SPACE))
    {
        for (auto *character_sprite: character_sprites.Sprites())
        {
            auto *character = dynamic_cast<Character *>(character_sprite);
            if (CheckConnections(100, &player, character))
            {
                player.Block();
                character->ChangeFacingDirection(player.rect.center());
                CreateDialog(character);
                character->can_rotate = false;
            }
        }
    }
    if (IsKeyPressed(rl::KEY_ENTER))
    {
        index_open = !index_open;
        player.blocked = !player.blocked;
    }
}

void Game::CreateDialog(Character *character)
{
    if (!dialog_tree.active)
    {
        dialog_tree = DialogTree(
                character, &player, all_sprites, &fonts["dialog"],
                [this](Character *char_)
                {
                    EndDialog(char_);
                });
    }
}

void Game::EndDialog(Character *character)
{
    dialog_tree.active = false;
    if (character->nurse)
    {
        for (auto &[i, monster]: player_monsters)
        {
            monster.health = monster.GetStat("max_health");
            monster.energy = monster.GetStat("max_energy");
        }
        player.Unblock();
    }
    else if (!character->character_data->defeated)
    {
        audio["overworld"].Stop();
        audio["battle"].Play();

        transition_target = TransitionTarget(TRANSITIONTARGET_LEVEL2BATTLE);
        transition_target.battle = Battle(
                &player_monsters, &character->monsters, &monster_frames, &outline_frames,
                &monster_icons, &ui_icons, &attack_frames,
                &bg_frames[character->character_data->biome], &fonts,
                [this](Character *c)
                {
                    EndBattle(c);
                }, character, &audio);
        tint_mode = TINT;
    }
    else
    {
        CheckEvolution();
        player.Unblock();
    }
}

void Game::TransitionCheck()
{
    std::vector<TransitionSprite *> sprites;
    // transition_sprites must contain only TransitionSprite
    for (auto *transition_sprite: transition_sprites.Sprites())
    {
        if (transition_sprite->rect.colliderect(player.hitbox))
        {
            sprites.push_back(dynamic_cast<TransitionSprite *>(transition_sprite));
        }
    }
    if (!sprites.empty())
    {
        player.Block();
        transition_target = TransitionTarget(TRANSITIONTARGET_MAP);
        transition_target.map_name = sprites[0]->target.first;
        transition_target.start_position = sprites[0]->target.second;
        tint_mode = TINT;
    }
}

void Game::TintScreen(const float dt)
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
            if (transition_target.target_type == TRANSITIONTARGET_LEVEL2BATTLE)
            {
                battle = std::move(transition_target.battle);
            }
            else if (transition_target.target_type == TRANSITIONTARGET_BATTLE2LEVEL)
            {
                battle.battle_over = true;
            }
            else if (transition_target.target_type == TRANSITIONTARGET_MAP)
            {
                Setup(transition_target.map_name, transition_target.start_position);
            }
            tint_mode = UNTINT;
        }
    }
    tint_progress = rl::Clamp(tint_progress, 0, 255);
    tint_surf.SetAlpha(tint_progress);
    display_surface->Blit(&tint_surf, rg::math::Vector2{0, 0});
}

void Game::EndBattle(Character *character)
{
    audio["battle"].Stop();

    transition_target = TransitionTarget(TRANSITIONTARGET_BATTLE2LEVEL);
    tint_mode = TINT;
    if (character)
    {
        character->character_data->defeated = true;
        CreateDialog(character);
    }
    else if (!evolution.IsActive())
    {
        player.Unblock();
        CheckEvolution();
    }
}

void Game::CheckMonster()
{
    if (!battle.battle_over || !player.direction || encounter_timer.active)
    {
        return;
    }
    for (const auto &sprite: monster_sprites.Sprites())
    {
        if (sprite->rect.colliderect(player.hitbox))
        {
            encounter_timer.Activate();
        }
    }
}

void Game::MonsterEncounter()
{
    if (!player.direction)
    {
        return;
    }
    for (const auto &sprite: monster_sprites.Sprites())
    {
        if (sprite->rect.colliderect(player.hitbox))
        {
            // change encounter timer duration for the next encounter
            encounter_timer.duration = float(rl::GetRandomValue(8, 25)) / 10.0f;

            const auto monster_patch_sprite = dynamic_cast<MonsterPatchSprite *>(sprite);

            player.Block();
            transition_target = TransitionTarget(TRANSITIONTARGET_LEVEL2BATTLE);

            encounter_monsters.clear();
            int count_monsters = 0;
            for (auto &monster_name: monster_patch_sprite->monsters)
            {
                const int level = monster_patch_sprite->level + rl::GetRandomValue(-3, 3);
                encounter_monsters[count_monsters++] =
                        Monster(monster_name, level);
            }

            audio["overworld"].Stop();
            audio["battle"].Play();

            transition_target.battle = Battle(
                    &player_monsters, &encounter_monsters, &monster_frames, &outline_frames,
                    &monster_icons, &ui_icons, &attack_frames,
                    &bg_frames[monster_patch_sprite->biome], &fonts,
                    [this](Character *c)
                    {
                        EndBattle(c);
                    }, nullptr, &audio);
            tint_mode = TINT;
            break; // only need the first sprite
        }
    }
}

void Game::CheckEvolution()
{
    for (auto &[index, monster]: player_monsters)
    {
        if (monster.evolution.second) // if evolution level
        {
            if (monster.level >= monster.evolution.second)
            {
                player.Block();

                audio["overworld"].Stop();
                audio["evolution"].Play();

                evolution = Evolution(
                        monster_frames, monster.name, monster.evolution.first, &fonts["bold"],
                        [this]
                        {
                            EndEvolution();
                        }, star_animation_surfs);
                player_monsters[index] =
                        Monster(monster.evolution.first, monster.level);
                break; // run the first evolution. When it is done we check if there are more
            }
        }
    }
    if (!evolution.IsActive())
    {
        audio["overworld"].Play();
    }
}

void Game::EndEvolution()
{
    audio["evolution"].Stop();
    audio["overworld"].Play();

    player.Unblock();
}
