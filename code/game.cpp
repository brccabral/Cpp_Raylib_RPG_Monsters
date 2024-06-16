#include <cstring>
#include <raylib.h>
#define RAYLIB_TMX_IMPLEMENTATION
#include "raylib-tmx.h"
#include "game.h"

#include "dialogtree.h"
#include "settings.h"
#include "sprite.h"
#include "entities.h"


Game::Game(const int width, const int height)
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(width, height, "RPG Monsters");
    // SetTargetFPS(60);

    display_surface = LoadRenderTexture(width, height);
    final_surface = LoadRenderTexture(width, height);

    ImporAssets();
    ClearSpriteGroups();

    Setup(tmx_maps["world"], "house");
    // Setup(tmx_maps["hospital"], "world");

    fonts["dialog"] = LoadFontEx("resources/graphics/fonts/PixeloidSans.ttf", FONT_SIZE, nullptr, 0);
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
    // create all_sprites after InitWindow for it uses LoadTexture
    all_sprites = new AllSprites;
    collition_sprites = new SpriteGroup;
    characters_sprites = new SpriteGroup;
    transition_sprites = new SpriteGroup;
}

void Game::Draw() const
{
    BeginTextureMode(display_surface);
    ClearBackground(BLACK);
    all_sprites->Draw(player);
    EndTextureMode();
}

void Game::run()
{
    while (!WindowShouldClose())
    {
        const double dt = GetFrameTime();
        Input();
        TransitionCheck();
        all_sprites->Update(dt);

        Draw();

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

        TintScreen(dt);

        DisplayUpdate();
    }
}

void Game::DisplayUpdate()
{
    BeginTextureMode(final_surface);
    DrawTexture(display_surface.texture, 0, 0, WHITE);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(final_surface.texture, 0, 0, render_tint);
    EndDrawing();
}

void Game::ImporAssets()
{
    tmx_maps["world"] = LoadTMX("resources/data/maps/world.tmx");
    tmx_maps["hospital"] = LoadTMX("resources/data/maps/hospital.tmx");
    tmx_maps["house"] = LoadTMX("resources/data/maps/house.tmx");

    overworld_frames["coast"] = {LoadTexture("resources/graphics/tilesets/coast.png")};
    named_textures["characters"] = ImportNamedFolder("resources/graphics/characters");
    named_rect_frames["coast"] = coast_rects();
    face_rect_frames["characters"] = all_character_import("resources/graphics/characters");
}

TileInfo Game::GetTileInfo(const tmx_tile *tile, const int posX, const int posY)
{
    TileInfo tile_info{};
    const tmx_image *im = tile->image;
    Texture2D *map_texture{};

    tile_info.position = {float(posX), float(posY)};

    Rectangle srcRect;
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
                auto [position, image] = GetTileInfo(map->tiles[gid], x * ts->tile_width, y * ts->tile_height);
                new Sprite(position, image, {all_sprites}, z);
            }
        }
    }
}

void Game::Setup(const tmx_map *map, const std::string &player_start_position)
{
    ClearSpriteGroups();

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
                auto [position, image] = GetTileInfo(map->tiles[gid], object->x, object->y - object->height);
                new Sprite(position, image, {all_sprites}, WORLD_LAYERS["top"]);
            }
            else
            {
                auto [position, image] = GetTileInfo(map->tiles[gid], object->x, object->y - object->height);
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
                {float(transition->x), float(transition->y)}, {float(transition->width), float(transition->height)},
                {target, pos}, {transition_sprites});
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
            auto [position, image] = GetTileInfo(map->tiles[gid], monster->x, monster->y - monster->height);
            std::string biome = tmx_get_property(monster->properties, "biome")->value.string;
            new MonsterPatchSprite(position, image, {all_sprites}, biome);
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
        if (strcmp(entity->name, "Player") == 0)
        {
            if (strcmp(tmx_get_property(entity->properties, "pos")->value.string, player_start_position.c_str()) == 0)
            {
                std::map<FacingDirection, std::vector<TiledTexture>> face_frames;
                std::string direction = tmx_get_property(entity->properties, "direction")->value.string;
                FacingDirection face_direction = face_name[direction];

                for (const auto &[key, rectangles]: face_rect_frames["characters"]["player"])
                {
                    for (const auto rect: rectangles)
                    {
                        face_frames[key].push_back({&named_textures["characters"]["player"], rect});
                    }
                }
                player = new Player(
                        {float(entity->x), float(entity->y)}, face_frames, {all_sprites}, face_direction,
                        collition_sprites);
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
        std::string character_id = tmx_get_property(entity->properties, "character_id")->value.string;
        int radius = std::stoi(tmx_get_property(entity->properties, "radius")->value.string);
        new Character(
                {float(entity->x), float(entity->y)}, face_frames, {all_sprites, collition_sprites, characters_sprites},
                face_direction, TRAINER_DATA[character_id], radius, this);

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
                        {float(x + water->x), float(y + water->y)}, frames, {all_sprites}, WORLD_LAYERS["water"]);
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
        new AnimatedSprite({float(coast->x), float(coast->y)}, frames, {all_sprites}, WORLD_LAYERS["bg"]);
        coast = coast->next;
    }
}

void Game::Input()
{
    if (dialog_tree)
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
}

void Game::UnloadResources()
{
    UnloadRenderTexture(display_surface);
    UnloadRenderTexture(final_surface);

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
    delete dialog_tree; // delete dialog_tree before all_sprites, it will remove itself
    delete all_sprites;
    delete collition_sprites;
    delete characters_sprites;
    delete transition_sprites;
}

void Game::CreateDialog(const Character *character)
{
    if (!dialog_tree)
    {
        dialog_tree = new DialogTree(
                character, player, {all_sprites}, fonts["dialog"], [this](const Character *ch) { EndDialog(ch); });
    }
}

// When there is no more phrases to show, unlock player
void Game::EndDialog(const Character *character)
{
    delete dialog_tree;
    dialog_tree = nullptr;
    player->Unblock();
}

void Game::TransitionCheck()
{
    std::vector<TransitionSprite *> sprites;
    for (const auto transition: transition_sprites->sprites)
    {
        auto *sprite = (TransitionSprite *) transition;
        if (CheckCollisionRecs(sprite->rect, player->hitbox))
        {
            sprites.push_back(sprite);
        }
    }
    if (!sprites.empty())
    {
        player->Block();
        transition_target = sprites[0]->target;
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
            Setup(tmx_maps[transition_target[0]], transition_target[1]);
            tint_mode = UNTINT;
            transition_target = {};
        }
    }
    render_tint = {255, 255, 255, (unsigned char) (tint_progress)};
}
