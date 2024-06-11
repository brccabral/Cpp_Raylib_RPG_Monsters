#pragma once

#include <filesystem>
#include <raylib.h>
#include <vector>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

inline std::vector<Texture2D> ImportFolder(const char *path)
{
    std::vector<Texture2D> frames;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        frames.push_back(LoadTexture(dirEntry.path().c_str()));
    }
    return frames;
}

typedef std::map<std::tuple<int, int>, Texture2D> tilemap_loc;

inline tilemap_loc ImportTilemap(const int cols, const int rows, const char *path)
{
    tilemap_loc frames;
    const Image surf = LoadImage(path);
    const int cell_width = surf.width / cols;
    const int cell_height = surf.height / rows;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            const Rectangle cutout_rect = {
                    float(col * cell_width), float(row * cell_height), float(cell_width), float(cell_height)};
            const Image cutout_surf = ImageFromImage(surf, cutout_rect);
            std::tuple<int, int> loc = {col, row};
            frames[loc] = LoadTextureFromImage(cutout_surf);
        }
    }
    return frames;
}

typedef std::map<std::string, std::vector<Texture2D>> tilemap_name;

inline std::map<std::string, tilemap_name> coast_importer(const int cols, const int rows, const char *path)
{
    auto frame_dict = ImportTilemap(cols, rows, path);
    std::map<std::string, tilemap_name> new_dict;
    const std::vector<std::string> terrains = {"grass", "grass_i", "sand_i", "sand", "rock", "rock_i", "ice", "ice_i"};
    const std::map<std::string, std::tuple<int, int>> sides = {
            {"topleft", {0, 0}}, //
            {"top", {1, 0}}, //
            {"topright", {2, 0}}, //
            {"left", {0, 1}}, //
            {"right", {2, 1}}, //
            {"bottomleft", {0, 2}}, //
            {"bottom", {1, 2}}, //
            {"bottomright", {2, 2}} //
    };
    for (int index = 0; index < terrains.size(); ++index)
    {
        const std::string &terrain = terrains[index];
        new_dict[terrain] = {};
        for (const auto &[key, pos]: sides)
        {
            std::vector<Texture2D> textures{};
            for (int row = 0; row < rows; row += 3)
            {
                auto frame = frame_dict[{std::get<0>(pos) + index * 3, std::get<1>(pos) + row}];
                textures.push_back(frame);
            }
            new_dict[terrain][key] = textures;
        }
    }

    return new_dict;
}
