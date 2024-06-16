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

    Game(int width, int height);
    ~Game();
    void Draw() const;
    void run();

    friend class Character; // allows Character to access Game private members

private:

    void ImporAssets();
    static TileInfo GetTileInfo(const tmx_tile *tile, int posX, int posY);
    void CreateTileLayer(const tmx_map *map, const tmx_layer *layer, int z = WORLD_LAYERS["main"]);
    void Setup(const tmx_map *map, const std::string &player_start_position);
    void Input();
    void UnloadResources();
    void CreateDialog(const Character *character);
    void EndDialog(const Character *character);
    void TransitionCheck();
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
};
