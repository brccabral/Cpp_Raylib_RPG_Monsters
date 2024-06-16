#pragma once
#include "dialogtree.h"
#include <map>
#include <string>
#include <tmx.h>
#include "entities.h"
#include "groups.h"
#include "support.h"


struct TileInfo
{
    Vector2 position;
    TiledTexture image;
};

class Game
{
public:

    void ClearSpriteGroups();
    Game(int width, int height);
    ~Game();
    void Draw() const;
    void run();

    friend class Character; // allows Character to access Game private members

private:

    void DisplayUpdate() const;
    void ImporAssets();
    static TileInfo GetTileInfo(const tmx_tile *tile, int posX, int posY);
    void CreateTileLayer(const tmx_map *map, const tmx_layer *layer, int z = WORLD_LAYERS["main"]);
    void Setup(const tmx_map *map, const std::string &player_start_position);
    void Input();
    void UnloadResources();
    void CreateDialog(const Character *character);
    void EndDialog(const Character *character);
    void TransitionCheck();
    void TintScreen(double dt);

    RenderTexture2D display_surface{};
    // BeginTextureMode draws everything upsidedown,
    // we need a second RenderTexture2D to invert it.
    // https://github.com/raysan5/raylib/issues/3803
    // https://github.com/raysan5/raylib/issues/378
    RenderTexture2D final_surface{};

    std::map<std::string, tmx_map *> tmx_maps;
    std::map<std::string, std::vector<Texture2D>> overworld_frames;
    std::map<std::string, std::map<std::string, Texture2D>> named_textures;
    std::map<std::string, std::map<std::string, tilerect_name>> named_rect_frames;
    std::map<std::string, std::map<std::string, tilerect_face>> face_rect_frames;
    // not *all*, but all that needs drawing or updates
    AllSprites *all_sprites = nullptr;
    SpriteGroup *collition_sprites = nullptr;
    SpriteGroup *characters_sprites = nullptr;
    SpriteGroup *transition_sprites = nullptr;
    Player *player{};
    std::map<std::string, Font> fonts;
    DialogTree *dialog_tree = nullptr;

    // Transition / tint
    Color render_tint = WHITE;
    std::array<std::string, 2> transition_target;
    TINT_MODE tint_mode = UNTINT;
    float tint_progress = 255;
    int tint_direction = -1;
    double tint_speed = 600;
};
