#pragma once
#include "raylib-tmx.h"
#include "battle.h"
#include "dialogtree.h"
#include "groups.h"
#include "monsterindex.h"
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
    void ClearSpriteGroups();

    friend class Character; // allows Character to access Game private members

    bool isRunning = true;

    std::map<int, Monster *> player_monsters;
    std::map<int, Monster *> dummy_monsters;
    std::map<std::string, std::map<std::string, Texture2D>> named_textures;
    std::map<std::string, Font> fonts;

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
    void SetupFrames();


    std::map<std::string, tmx_map *> tmx_maps;
    std::map<std::string, std::vector<Texture2D>> overworld_frames;
    std::map<std::string, std::map<std::string, tilerect_name>> named_rect_frames;
    std::map<std::string, std::map<std::string, tilerect_face>> face_rect_frames;
    std::map<std::string, animation_rects> animation_frames;
    AttackAnimationRects attack_animation_rects;
    std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>> monsters_frames;
    std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>> outline_frames;
    std::map<AttackAnimation, std::vector<TiledTexture>> attack_animation_frames;

    // not *all*, but all that needs drawing or updates
    AllSprites *all_sprites = nullptr;
    SpriteGroup *collition_sprites = nullptr;
    SpriteGroup *characters_sprites = nullptr;
    SpriteGroup *transition_sprites = nullptr;
    Player *player{};

    // overlays
    DialogTree *dialog_tree = nullptr;
    MonsterIndex *monster_index = nullptr;
    Battle *battle = nullptr;
    bool index_open = false;

    // Transition / tint
    Color render_tint = WHITE;
    std::array<std::string, 2> transition_target;
    TINT_MODE tint_mode = UNTINT;
    float tint_progress = 255;
    int tint_direction = -1;
    double tint_speed = 600;
};
