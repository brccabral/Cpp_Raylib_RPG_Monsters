#pragma once
#include <map>
#include <string>
#include <utility>
#include <raylib.h>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TILE_SIZE = 64;
constexpr int ANIMATION_SPEED = 6;
constexpr int BATTLE_OUTLINE_WIDTH = 4;

inline std::map<std::string, Color> COLORS = {
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
};

inline std::map<std::string, int> WORLD_LAYERS = {
        {"water", 0}, //
        {"bg", 1}, //
        {"shadow", 2}, //
        {"main", 3}, //
        {"top", 4}, //
};

inline std::map<std::string, std::map<std::string, Vector2>> BATTLE_POSITIONS = {
        //
        {
                "left",
                {
                        //
                        {"top", {360, 260}}, //
                        {"center", {190, 400}}, //
                        {"bottom", {410, 520}} //
                } //
        }, //
        {
                "right",
                {
                        //
                        {"top", {900, 260}}, //
                        {"center", {1110, 390}}, //
                        {"bottom", {900, 550}} //
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

typedef struct BattleChoice
{
    Vector2 pos;
    std::string icon;
} BattleChoice;

inline std::map<std::string, std::map<std::string, BattleChoice>> BATTLE_CHOICES = {
        //
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

inline int FONT_SIZE = 30;

enum SpriteType
{
    SIMPLESPRITE = 0,
    SPRITE,
    ENTITY,
    DIALOGSPRITE
};

enum Axis
{
    HORIZONTAL = 0,
    VERTICAL
};

enum FacingDirection
{
    DOWN = 0,
    UP,
    LEFT,
    RIGHT,
    DOWN_IDLE,
    UP_IDLE,
    LEFT_IDLE,
    RIGHT_IDLE,
};

enum TINT_MODE
{
    UNTINT = 0,
    TINT
};
