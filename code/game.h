#pragma once
#include "raylib-tmx.h"
#include "battle.h"
#include "dialogtree.h"
#include "evolution.h"
#include "groups.h"
#include "monsterindex.h"
#include "support.h"
#include "transitiontarget.h"


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
    void EndBattle(Character *character);
    void EndEvolution();
    void CreateDialog(Character *character);
    void SetCurrentBackgroundMusic(const Music &newMusic);

    bool isRunning = true;

    std::map<int, Monster> player_monsters;
    std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>> monsters_frames;
    std::map<std::string, std::map<AnimationState, std::vector<TiledTexture>>> outline_frames;
    std::map<AttackAnimation, std::vector<TiledTexture>> attack_animation_frames;
    std::map<std::string, std::map<std::string, Texture2D>> named_textures;
    std::map<std::string, Font> fonts;
    Player *player{};
    SpriteGroup *collition_sprites = nullptr;

    // audio
    std::map<std::string, Music> musics;
    std::map<std::string, Sound> sounds;

private:

    void DisplayUpdate() const;
    void ImporAssets();
    static TileInfo GetTileInfo(const tmx_tile *tile, int posX, int posY);
    void CreateTileLayer(const tmx_map *map, const tmx_layer *layer, int z = WORLD_LAYERS["main"]);
    void Setup(const std::string &map_name, const std::string &player_start_position);
    void Input();
    void UnloadResources();
    void EndDialog(Character *character);
    void TransitionCheck();
    void TintScreen(double dt);
    void SetupFrames();
    // monster encounters
    void CheckMonster();
    void MonsterEncounter();
    void CheckEvolution();

    std::map<std::string, tmx_map *> tmx_maps;
    std::map<std::string, std::vector<Texture2D>> overworld_frames;
    std::map<std::string, std::map<std::string, tilerect_name>> named_rect_frames;
    std::map<std::string, std::map<std::string, tilerect_face>> face_rect_frames;
    std::map<std::string, animation_rects> animation_frames;
    AttackAnimationRects attack_animation_rects;
    std::vector<Texture2D> star_animation_textures;

    // not *all*, but all that needs drawing or updates
    AllSprites *all_sprites = nullptr;
    SpriteGroup *characters_sprites = nullptr;
    SpriteGroup *transition_sprites = nullptr;
    SpriteGroup *monster_sprites = nullptr; // monster encounter
    std::map<int, Monster *> encounter_monsters;
    Evolution *evolution = nullptr;

    // overlays
    DialogTree *dialog_tree = nullptr;
    MonsterIndex *monster_index = nullptr;
    Battle *battle = nullptr;
    bool index_open = false;

    // Transition / tint
    Color render_tint = WHITE;
    TransitionTarget *transition_target = nullptr;
    TINT_MODE tint_mode = UNTINT;
    float tint_progress = 255;
    int tint_direction = -1;
    double tint_speed = 600;

    Timer encounter_timer;

    // audio
    Music current_music{};
};
