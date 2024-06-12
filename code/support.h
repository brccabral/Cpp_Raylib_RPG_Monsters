#pragma once

#include <filesystem>
#include <iostream>
#include <map>
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
            for (int row = 0; row < rows; row += 3)
            {
                new_dict[terrain][key].push_back(frame_dict[{std::get<0>(pos) + index * 3, std::get<1>(pos) + row}]);
            }
        }
    }

    return new_dict;
}

inline tilemap_name CharacterImporter(const int cols, const int rows, const char *path)
{
    auto frame_dic = ImportTilemap(cols, rows, path);
    tilemap_name new_dic = {};
    const std::vector<std::string> directions = {"down", "left", "right", "up"};
    for (int row = 0; row < directions.size(); ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            new_dic[directions[row]].push_back(frame_dic[std::tuple<int, int>{col, row}]);
            std::string idle_name = directions[row] + "_idle";
            new_dic[idle_name] = {frame_dic[std::tuple<int, int>{0, row}]};
        }
    }
    return new_dic;
}

inline std::map<std::string, tilemap_name> all_character_import(const char *path)
{
    std::map<std::string, tilemap_name> new_dict = {};

    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        if (dirEntry.is_regular_file())
        {
            auto filename = dirEntry.path().stem().string();
            new_dict[filename] = CharacterImporter(4, 4, dirEntry.path().c_str());
        }
    }

    return new_dict;
}
