#include <cstring>
#include <ctime>
#include <raylib.h>
#define RAYLIB_TMX_IMPLEMENTATION
#include "raylib-tmx.h"
#include "game.h"
#include "raylib_utils.h"

#include <iostream>


Game::Game(const int width, const int height)
{
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(width, height, "RPG Monsters");
    // SetTargetFPS(60);
    SetRandomSeed(std::time(nullptr));
    SetExitKey(KEY_NULL); // Disable KEY_ESCAPE to close window, X-button still works

    display_surface = LoadRenderTexture(width, height);
    while (!IsRenderTextureReady(display_surface))
    {}

    ImporAssets();
    ClearSpriteGroups();

    Setup("world", "house");
    SetupFrames();

    encounter_timer = new Timer(2.0f, false, false, [this] { MonsterEncounter(); });
}

Game::~Game()
{
    UnloadResources();
    CloseWindow();
}

void Game::ClearSpriteGroups()
{
    if (all_sprites)
    {
        delete all_sprites;
        all_sprites = nullptr;
    }
    if (collition_sprites)
    {
        delete collition_sprites;
        collition_sprites = nullptr;
    }
    if (characters_sprites)
    {
        delete characters_sprites;
        characters_sprites = nullptr;
    }
    if (transition_sprites)
    {
        delete transition_sprites;
        transition_sprites = nullptr;
    }
    if (monster_sprites)
    {
        delete monster_sprites;
        monster_sprites = nullptr;
    }
    // create all_sprites after InitWindow for it uses LoadTexture
    all_sprites = new AllSprites;
    collition_sprites = new SpriteGroup;
    characters_sprites = new SpriteGroup;
    transition_sprites = new SpriteGroup;
    monster_sprites = new SpriteGroup;
}

void Game::Draw() const
{
    BeginTextureModeC(display_surface, BLACK);
    all_sprites->Draw(player);
    EndTextureModeSafe();
}

void Game::run()
{
    while (!WindowShouldClose() && isRunning)
    {
        const double dt = GetFrameTime();

        // update
        encounter_timer->Update();
        Input();
        TransitionCheck();
        if (!battle)
        {
            all_sprites->Update(dt);

            // drawing
            Draw();
        }
        CheckMonster();

        // overlays
        // dialog_tree checks for SPACE input, which conflicts
        // with Game input.
        // If player is next to a trainer, the Game.Input opens the first
        // dialog, and dialog_tree opens the second.
        // The tutorial uses a Timer to avoid the conflict
        // The tutorial puts dialog after the drawing, but raylib
        // resets all KEY status after EndDrawing. So, we need all
        // inputs before drawing.
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
        // TODO remove this
        if (!evolution)
            CheckEvolution();

        TintScreen(dt);

        DisplayUpdate();
    }
}

void Game::DisplayUpdate() const
{
    BeginDrawingC(BLACK);

    // RenderTexture renders things flipped in Y axis, we draw it "unflipped"
    // https://github.com/raysan5/raylib/issues/3803
    // https://github.com/raysan5/raylib/issues/378=
    DrawTextureRec(
            display_surface.texture,
            {0, 0, (float) display_surface.texture.width, (float) -display_surface.texture.height},
            {0, 0}, render_tint);

#if 0
    const auto [mouse_x, mouse_y] = GetMousePosition();
    const char *mouse_text = TextFormat("%.f / %.f", mouse_x, mouse_y);
    DrawCircle((int) mouse_x, (int) mouse_y, 4.0f, BLACK);
    DrawRectangle(mouse_x, mouse_y, 60, 15, WHITE);
    DrawText(mouse_text, (int) mouse_x + 5, (int) mouse_y + 5, 10, BLACK);
#endif

    EndDrawing();
}

void Game::ImporAssets()
{
    tmx_maps = tmx_importer("resources/data/maps");

    overworld_frames["coast"] = {LoadTexture("resources/graphics/tilesets/coast.png")};

    named_textures["characters"] = ImportNamedFolder("resources/graphics/characters");
    named_textures["monsters"] = ImportNamedFolder("resources/graphics/monsters");
    named_textures["icons"] = ImportNamedFolder("resources/graphics/icons");
    named_textures["ui"] = ImportNamedFolder("resources/graphics/ui");
    named_textures["bg_frames"] = ImportNamedFolder("resources/graphics/backgrounds");
    named_textures["outlines"] = OutlineCreator(named_textures["monsters"], 4);
    named_textures["attacks"] = ImportNamedFolder("resources/graphics/attacks");

    named_rect_frames["coast"] = coast_rects();
    animation_frames = MonsterImporter(4, 2, "resources/graphics/monsters");
    attack_animation_rects = AttackImporter(4, 1, "resources/graphics/attacks");

    face_rect_frames["characters"] = all_character_import("resources/graphics/characters");

    fonts["dialog"] =
            LoadFontEx("resources/graphics/fonts/PixeloidSans.ttf", FONT_SIZE, nullptr, 0);
    fonts["regular"] = LoadFontEx("resources/graphics/fonts/PixeloidSans.ttf", 18, nullptr, 0);
    fonts["small"] = LoadFontEx("resources/graphics/fonts/PixeloidSans.ttf", 14, nullptr, 0);
    fonts["bold"] = LoadFontEx("resources/graphics/fonts/dogicapixelbold.otf", 20, nullptr, 0);
}

TileInfo Game::GetTileInfo(const tmx_tile *tile, const int posX, const int posY)
{
    TileInfo tile_info{};
    const tmx_image *im = tile->image;
    Texture2D *map_texture{};

    tile_info.position = {float(posX), float(posY)};

    RectangleU srcRect;
    srcRect.x = tile->ul_x;
    srcRect.y = tile->ul_y;
    srcRect.width = tile->width;
    srcRect.height = tile->height;

    tile_info.image.rect = srcRect;

    if (im && im->resource_image)
    {
        map_texture = (Texture2D *) im->resource_image;
    }
    else if (tile->tileset->image->resource_image)
    {
        map_texture = (Texture2D *) tile->tileset->image->resource_image;
    }
    if (map_texture)
    {
        tile_info.image.texture = map_texture;
    }
    return tile_info;
}

void Game::CreateTileLayer(const tmx_map *map, const tmx_layer *layer, const int z)
{
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            const unsigned int baseGid = layer->content.gids[(y * map->width) + x];
            const unsigned int gid = (baseGid) &TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid])
            {
                const tmx_tileset *ts = map->tiles[gid]->tileset;
                auto [position, image] =
                        GetTileInfo(map->tiles[gid], x * ts->tile_width, y * ts->tile_height);
                new Sprite(position, image, {all_sprites}, z);
            }
        }
    }
}

void Game::Setup(const std::string &map_name, const std::string &player_start_position)
{
    ClearSpriteGroups();

    const tmx_map *map = tmx_maps[map_name];

    const tmx_layer *terrain_layer = tmx_find_layer_by_name(map, "Terrain");
    const tmx_layer *entities_layer = tmx_find_layer_by_name(map, "Entities");
    const tmx_layer *objects_layer = tmx_find_layer_by_name(map, "Objects");
    const tmx_layer *terrain_top_layer = tmx_find_layer_by_name(map, "Terrain Top");
    const tmx_layer *water_layer = tmx_find_layer_by_name(map, "Water");
    const tmx_layer *coast_layer = tmx_find_layer_by_name(map, "Coast");
    const tmx_layer *monster_layer = tmx_find_layer_by_name(map, "Monsters");
    const tmx_layer *collisions_layer = tmx_find_layer_by_name(map, "Collisions");
    const tmx_layer *transition_layer = tmx_find_layer_by_name(map, "Transition");

    CreateTileLayer(map, terrain_layer, WORLD_LAYERS["bg"]);
    CreateTileLayer(map, terrain_top_layer, WORLD_LAYERS["bg"]);

    auto object = objects_layer->content.objgr->head;
    while (object)
    {
        const int gid = object->content.gid;
        if (map->tiles[gid])
        {
            std::string name;
            if (object->name)
            {
                name = object->name;
            }
            if (strcmp(name.c_str(), "top") == 0)
            {
                auto [position, image] =
                        GetTileInfo(map->tiles[gid], object->x, object->y - object->height);
                new Sprite(position, image, {all_sprites}, WORLD_LAYERS["top"]);
            }
            else
            {
                auto [position, image] =
                        GetTileInfo(map->tiles[gid], object->x, object->y - object->height);
                new CollidableSprite(position, image, {all_sprites, collition_sprites});
            }
        }
        object = object->next;
    }

    auto transition = transition_layer->content.objgr->head;
    while (transition)
    {
        std::string target = tmx_get_property(transition->properties, "target")->value.string;
        std::string pos = tmx_get_property(transition->properties, "pos")->value.string;
        new TransitionSprite(
                {float(transition->x), float(transition->y)},
                {float(transition->width), float(transition->height)}, target, pos,
                {transition_sprites});
        transition = transition->next;
    }

    auto collision = collisions_layer->content.objgr->head;
    while (collision)
    {
        TiledTexture image{};
        image.rect = {0, 0, float(collision->width), float(collision->height)};
        new BorderSprite({float(collision->x), float(collision->y)}, image, {collition_sprites});

        collision = collision->next;
    }

    auto monster = monster_layer->content.objgr->head;
    while (monster)
    {
        const int gid = monster->content.gid;
        if (map->tiles[gid])
        {
            auto [position, image] =
                    GetTileInfo(map->tiles[gid], monster->x, monster->y - monster->height);
            std::string biome = tmx_get_property(monster->properties, "biome")->value.string;
            std::string monsters = tmx_get_property(monster->properties, "monsters")->value.string;
            int level = tmx_get_property(monster->properties, "level")->value.integer;
            new MonsterPatchSprite(
                    position, image, {all_sprites, monster_sprites}, biome, monsters, level);
        }
        monster = monster->next;
    }

    std::map<std::string, FacingDirection> face_name = {
            {"down", DOWN},
            {"left", LEFT},
            {"up", UP},
            {"right", RIGHT},
    };

    // we need to find the player first, so it can be passed to all characters in the next look
    auto entity = entities_layer->content.objgr->head;
    while (entity)
    {
        if (std::strcmp(entity->name, "Player") == 0)
        {
            char *entity_pos = tmx_get_property(entity->properties, "pos")->value.string;
            if (std::strcmp(entity_pos, player_start_position.c_str()) == 0)
            {
                std::map<FacingDirection, std::vector<TiledTexture>> face_frames;
                std::string direction =
                        tmx_get_property(entity->properties, "direction")->value.string;
                FacingDirection face_direction = face_name[direction];

                for (const auto &[key, rectangles]: face_rect_frames["characters"]["player"])
                {
                    for (const auto rect: rectangles)
                    {
                        face_frames[key].push_back({&named_textures["characters"]["player"], rect});
                    }
                }
                player = new Player(
                        {float(entity->x), float(entity->y)}, face_frames, {all_sprites},
                        face_direction, collition_sprites);
                break;
            }
        }

        entity = entity->next;
    }

    // we can only create Characters after we have Player
    entity = entities_layer->content.objgr->head;
    while (entity)
    {
        if (strcmp(entity->name, "Player") == 0)
        {
            entity = entity->next;
            continue;
        }
        std::map<FacingDirection, std::vector<TiledTexture>> face_frames;
        std::string direction = tmx_get_property(entity->properties, "direction")->value.string;
        FacingDirection face_direction = face_name[direction];

        std::string graphic = tmx_get_property(entity->properties, "graphic")->value.string;
        for (const auto &[key, rectangles]: face_rect_frames["characters"][graphic])
        {
            for (const auto rect: rectangles)
            {
                face_frames[key].push_back({&named_textures["characters"][graphic], rect});
            }
        }
        std::string character_id =
                tmx_get_property(entity->properties, "character_id")->value.string;
        int radius = std::stoi(tmx_get_property(entity->properties, "radius")->value.string);
        bool nurse = std::strcmp(character_id.c_str(), "Nurse") == 0;
        new Character(
                {float(entity->x), float(entity->y)}, face_frames,
                {all_sprites, collition_sprites, characters_sprites}, face_direction,
                TRAINER_DATA[character_id], radius, this, nurse);

        entity = entity->next;
    }

    auto water = water_layer->content.objgr->head;
    while (water)
    {
        for (int y = 0; y < water->height; y += TILE_SIZE)
        {
            for (int x = 0; x < water->width; x += TILE_SIZE)
            {
                std::vector<TiledTexture> frames;
                for (const auto rect: named_rect_frames["coast"]["grass"]["middle"])
                {
                    frames.push_back({&overworld_frames["coast"][0], rect});
                }
                new AnimatedSprite(
                        {float(x + water->x), float(y + water->y)}, frames, {all_sprites},
                        WORLD_LAYERS["water"]);
            }
        }
        water = water->next;
    }

    auto coast = coast_layer->content.objgr->head;
    while (coast)
    {
        std::string terrain = tmx_get_property(coast->properties, "terrain")->value.string;
        std::string side = tmx_get_property(coast->properties, "side")->value.string;

        std::vector<TiledTexture> frames;
        for (const auto rect: named_rect_frames["coast"][terrain][side])
        {
            frames.push_back({&overworld_frames["coast"][0], rect});
        }
        new AnimatedSprite(
                {float(coast->x), float(coast->y)}, frames, {all_sprites}, WORLD_LAYERS["bg"]);
        coast = coast->next;
    }
}

void Game::Input()
{
    if (dialog_tree || battle)
    {
        return;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        for (auto *sprite: characters_sprites->sprites)
        {
            auto *character = (Character *) sprite;
            if (CheckConnections(100, player, character))
            {
                player->Block();
                character->ChangeFacingDirection(GetRectCenter(player->rect));
                CreateDialog(character);
                character->can_rotate = false;
            }
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        index_open = !index_open;
        player->blocked = !player->blocked;
    }
}

void Game::UnloadResources()
{
    UnloadRenderTexture(display_surface);

    for (const auto &[key, map]: tmx_maps)
    {
        UnloadTMX(map);
    }

    for (const auto &[key, textures]: overworld_frames)
    {
        for (const auto texture: textures)
        {
            UnloadTexture(texture);
        }
    }

    for (const auto &[key, named_texture]: named_textures)
    {
        for (const auto &[name, texture]: named_texture)
        {
            UnloadTexture(texture);
        }
    }

    for (auto &[key, font]: fonts)
    {
        UnloadFont(font);
    }

    delete dialog_tree; // delete dialog_tree before all_sprites, it will remove itself
    delete transition_target;
    delete all_sprites;
    delete collition_sprites;
    delete characters_sprites;
    delete transition_sprites;
    delete monster_sprites;
    delete monster_index;
    delete battle;
    delete encounter_timer;

    if (evolution)
    {
        delete evolution;
        evolution = nullptr;
    }

    for (const auto &[index, monster]: player_monsters)
    {
        delete monster;
    }

    if (!encounter_monsters.empty())
    {
        for (auto &[i, monster]: encounter_monsters)
        {
            delete monster;
        }
        encounter_monsters.clear();
    }
}

void Game::CreateDialog(Character *character)
{
    if (!dialog_tree)
    {
        dialog_tree = new DialogTree(
                character, player, {all_sprites}, fonts["dialog"],
                [this](Character *ch) { EndDialog(ch); });
    }
}

// When there is no more phrases to show, unlock player
void Game::EndDialog(Character *character)
{
    delete dialog_tree;
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
    else if (!character->character_data.defeated)
    {
        const Texture2D bg = named_textures["bg_frames"][character->character_data.biome];
        if (transition_target)
        {
            delete transition_target;
            transition_target = nullptr;
        }
        transition_target = new TransitionTarget(TRANSITIONTARGET_LEVEL2BATTLE);
        transition_target->battle = new Battle(this, &character->monsters, bg, character);
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
    std::vector<TransitionSprite *> sprites;
    for (const auto transition: transition_sprites->sprites)
    {
        auto *sprite = (TransitionSprite *) transition;
        if (CheckCollisionRecs(sprite->rect.rectangle, player->hitbox.rectangle))
        {
            sprites.push_back(sprite);
        }
    }
    if (!sprites.empty())
    {
        player->Block();
        transition_target = new TransitionTarget(TRANSITIONTARGET_MAP);
        transition_target->map_name = sprites[0]->map_name;
        transition_target->start_position = sprites[0]->start_position;
        tint_mode = TINT;
    }
}

// Fade to black
void Game::TintScreen(const double dt)
{
    // in the tutorial the tint is "transparent to black"
    // in raylib we do "white to transparent"

    if (tint_mode == UNTINT)
    {
        tint_progress += tint_speed * dt;
        if (tint_progress >= 255)
        {
            tint_progress = 255;
        }
    }
    if (tint_mode == TINT)
    {
        tint_progress -= tint_speed * dt;
        if (tint_progress <= 0)
        {
            tint_progress = 0;
            if (transition_target->target_type == TRANSITIONTARGET_LEVEL2BATTLE)
            {
                battle = transition_target->battle;
            }
            else if (transition_target->target_type == TRANSITIONTARGET_BATTLE2LEVEL)
            {
                delete battle;
                battle = nullptr;
            }
            else if (transition_target->target_type == TRANSITIONTARGET_MAP)
            {
                Setup(transition_target->map_name, transition_target->start_position);
            }
            tint_mode = UNTINT;
            delete transition_target;
            transition_target = nullptr;
        }
    }
    render_tint = {255, 255, 255, (unsigned char) (tint_progress)};
}

void Game::SetupFrames()
{

    int player_index = 0;
    player_monsters[player_index++] = new Monster("Charmadillo", 30);
    player_monsters[player_index++] = new Monster("Friolera", 29);
    player_monsters[player_index++] = new Monster("Larvea", 4);
    player_monsters[player_index++] = new Monster("Atrox", 24);
    player_monsters[player_index++] = new Monster("Sparchu", 24);
    player_monsters[player_index++] = new Monster("Gulfin", 24);
    player_monsters[player_index++] = new Monster("Jacana", 2);
    player_monsters[player_index++] = new Monster("Pouch", 3);

    for (const auto &[monster_name, animations]: animation_frames)
    {
        for (const auto &[key, frames]: animations)
        {
            for (const auto frame: frames)
            {
                monsters_frames[monster_name][key].push_back(
                        {&named_textures["monsters"][monster_name], frame});

                outline_frames[monster_name][key].push_back(
                        {&named_textures["outlines"][monster_name], frame});
            }
        }
    }
    monster_index = new MonsterIndex(
            &player_monsters, fonts, named_textures["icons"], monsters_frames,
            named_textures["ui"]);

    for (auto &[attack_name, attack_texture]: named_textures["attacks"])
    {
        AttackAnimation animation = ATTACK_ANIMATION_NAMES[attack_name];
        for (const auto attack_rect: attack_animation_rects[animation])
        {
            attack_animation_frames[animation].push_back({&attack_texture, attack_rect});
        }
    }
}

void Game::CheckMonster() const
{
    for (const auto sprite: monster_sprites->sprites)
    {
        if (!battle && player->IsMoving() &&
            CheckCollisionRecs(sprite->rect.rectangle, player->hitbox.rectangle))
        {
            if (!encounter_timer->active)
            {
                encounter_timer->Activate();
            }
        }
    }
}

void Game::MonsterEncounter()
{
    for (const auto sprite: monster_sprites->sprites)
    {
        if (player->IsMoving() &&
            CheckCollisionRecs(sprite->rect.rectangle, player->hitbox.rectangle))
        {
            // change encounter timer duration for the next encounter
            encounter_timer->duration = GetRandomValue(8, 25) / 10.0f;

            const auto *monster_patch_sprite = (MonsterPatchSprite *) sprite;
            int count_mounstes = 0;
            if (!encounter_monsters.empty())
            {
                for (auto &[i, monster]: encounter_monsters)
                {
                    delete monster;
                }
                encounter_monsters.clear();
            }
            for (const auto &monster_name: monster_patch_sprite->monsters)
            {
                const int level = monster_patch_sprite->level + GetRandomValue(-3, 3);
                encounter_monsters[count_mounstes++] = new Monster(monster_name, level);
            }
            player->Block();
            Texture2D bg = named_textures["bg_frames"][monster_patch_sprite->biome];
            if (transition_target)
            {
                delete transition_target;
                transition_target = nullptr;
            }
            transition_target = new TransitionTarget(TRANSITIONTARGET_LEVEL2BATTLE);
            transition_target->battle = new Battle(this, &encounter_monsters, bg, nullptr);
            tint_mode = TINT;
        }
    }
}

void Game::CheckEvolution()
{
    if (evolution)
    {
        return;
    }
    for (auto [index, monster]: player_monsters)
    {
        if (monster->evolve.second)
        {
            if (monster->level >= monster->evolve.second)
            {
                player->Block();
                evolution = new Evolution(
                        &named_textures["monsters"], &animation_frames, monster->name,
                        monster->evolve.first, fonts["bold"], [this] { EndEvolution(); });
                break;
            }
        }
    }
}

void Game::EndBattle(Character *character)
{
    if (transition_target)
    {
        delete transition_target;
        transition_target = nullptr;
    }
    transition_target = new TransitionTarget(TRANSITIONTARGET_BATTLE2LEVEL);
    tint_mode = TINT;
    if (character)
    {
        character->character_data.defeated = true;
        CreateDialog(character);
    }
    else if (!evolution)
    {
        player->Unblock();
        CheckEvolution();
    }
}

void Game::EndEvolution()
{
    if (evolution)
    {
        delete evolution;
        evolution = nullptr;
    }
    player->Unblock();
}
