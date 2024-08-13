#pragma once


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
