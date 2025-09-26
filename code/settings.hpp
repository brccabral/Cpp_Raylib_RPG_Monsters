#pragma once
#include <rygame.hpp>


constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TILE_SIZE = 64;
constexpr float ANIMATION_SPEED = 6;

enum SelectionMode
{
    SELECTMODE_NONE = 0,
    SELECTMODE_GENERAL,
    SELECTMODE_MONSTER,
    SELECTMODE_ATTACKS,
    SELECTMODE_SWITCH,
    SELECTMODE_TARGET,
};

typedef struct BattleChoice
{
    rg::math::Vector2 pos;
    std::string icon;
} BattleChoice;

enum AnimationState
{
    ANIMATIONSTATE_IDLE = 0,
    ANIMATIONSTATE_ATTACK,
};

enum TINT_MODE
{
    UNTINT = 0,
    TINT
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
inline rg::InsertOrderMap<std::string, int> WORLD_LAYERS = {
        {"water", 0}, //
        {"bg", 1}, //
        {"shadow", 2}, //
        {"main", 3}, //
        {"top", 4}, //
};

inline std::map<std::string, rl::Color> COLORS = {
        {"white", {0xf4, 0xfe, 0xfa, 0xff}}, //
        {"pure white", {0xff, 0xff, 0xff, 0xff}}, //
        {"dark", {0x2b, 0x29, 0x2c, 0xff}}, //
        {"light", {0xc8, 0xc8, 0xc8, 0xff}}, //
        {"gray", {0x3a, 0x37, 0x3b, 0xff}}, //
        {"gold", {0xff, 0xd7, 0x00, 0xff}}, //
        {"light-gray", {0x4b, 0x48, 0x4d, 0xff}}, //
        {"fire", {0xf8, 0xa0, 0x60, 0xff}}, //
        {"water", {0x50, 0xb0, 0xd8, 0xff}}, //
        {"plant", {0x64, 0xa9, 0x90, 0xff}}, //
        {"black", {0x00, 0x00, 0x00, 0xff}}, //
        {"red", {0xf0, 0x31, 0x31, 0xff}}, //
        {"blue", {0x66, 0xd7, 0xee, 0xff}}, //
        {"normal", {0xff, 0xff, 0xff, 0xff}}, //
        {"dark white", {0xf0, 0xf0, 0xf0, 0xff}}, //
};

inline std::map<std::string, std::vector<rg::math::Vector2>> BATTLE_POSITIONS = {
        {
                "left",
                {
                        {360, 260}, // top
                        {190, 400}, // center
                        {410, 520} // bottom
                } //
        }, //
        {
                "right",
                {
                        {900, 260}, // top
                        {1110, 390}, // center
                        {900, 550} // bottom
                } //
        } //
};

inline std::map<std::string, int> BATTLE_LAYERS = {
        {"outline", 0}, //
        {"name", 1}, //
        {"monster", 2}, //
        {"effects", 3}, //
        {"overlay", 4}, //
};

inline std::map<std::string, std::vector<std::pair<std::string, BattleChoice>>> BATTLE_CHOICES{
        // numbers adjusted from tutorial
        {
                "full",
                {
                        //
                        {"fight", {{30, -60}, "sword"}}, //
                        {"defend", {{40, -20}, "shield"}}, //
                        {"switch", {{40, 20}, "arrows"}}, //
                        {"catch", {{30, 60}, "hand"}}, //
                }, //
        }, //
        {
                "limited",
                {
                        //
                        {"fight", {{30, -40}, "sword"}}, //
                        {"defend", {{40, 0}, "shield"}}, //
                        {"switch", {{30, 40}, "arrows"}}, //
                }, //
        }, //
};
#pragma clang diagnostic pop
