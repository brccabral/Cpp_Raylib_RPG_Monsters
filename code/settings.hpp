#pragma once
#include <rygame.hpp>


constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TILE_SIZE = 64;
constexpr float ANIMATION_SPEED = 6;

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
