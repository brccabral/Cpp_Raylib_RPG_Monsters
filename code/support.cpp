#include <sstream>
#include <filesystem>
#include "support.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

std::vector<Texture2D> ImportFolder(const char *path)
{
    std::vector<Texture2D> frames;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto entryPath = dirEntry.path().string();
        frames.push_back(LoadTexture(entryPath.c_str()));
    }
    return frames;
}

std::map<std::string, Texture2D> ImportNamedFolder(const char *path)
{
    std::map<std::string, Texture2D> textures;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        textures[filename] = LoadTexture(entryPath.c_str());
    }
    return textures;
}

std::map<std::string, tilerect_name> coast_rects()
{
    std::map<std::string, tilerect_name> new_dict;
    const std::vector<std::string> terrains = {"grass", "grass_i", "sand_i", "sand",
                                               "rock",  "rock_i",  "ice",    "ice_i"};
    const std::map<std::string, std::tuple<int, int>> sides = {
            {"topleft", {0, 0}}, //
            {"top", {1, 0}}, //
            {"topright", {2, 0}}, //
            {"left", {0, 1}}, //
            {"middle", {1, 1}}, //
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
            for (int row = 0; row < 4; ++row)
            {
                new_dict[terrain][key].push_back(
                        {(std::get<0>(pos) + index * 3) * 64.0f,
                         (std::get<1>(pos) + row * 3) * 64.0f, 64.0f, 64.0f});
            }
        }
    }

    return new_dict;
}

std::vector<std::vector<RectangleU>>
import_tilemap_rects(const int cols, const int rows, const char *path)
{
    std::vector<std::vector<RectangleU>> frames;
    const Texture2D surf = LoadTexture(path);

    const auto cell_width = float(surf.width) / cols;
    const auto cell_height = float(surf.height) / rows;

    for (int row = 0; row < rows; ++row)
    {
        frames.emplace_back();
        for (int col = 0; col < cols; ++col)
        {
            frames[row].push_back({col * cell_width, row * cell_height, cell_width, cell_height});
        }
    }
    UnloadTexture(surf);
    return frames;
}

tilerect_face CharacterImporter(const int cols, const int rows, const char *path)
{
    std::vector<std::vector<RectangleU>> frames = import_tilemap_rects(cols, rows, path);
    tilerect_face new_dic = {};
    // same order as in image set
    const std::vector directions = {DOWN, LEFT, RIGHT, UP};
    const std::vector idle_directions = {DOWN_IDLE, LEFT_IDLE, RIGHT_IDLE, UP_IDLE};
    for (int row = 0; row < directions.size(); ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            new_dic[directions[row]].push_back(frames[row][col]);
        }
        FacingDirection idle_name = idle_directions[row];
        new_dic[idle_name] = {frames[row][0]};
    }
    return new_dic;
}

std::map<std::string, animation_rects>
MonsterImporter(const int cols, const int rows, const char *path)
{
    std::map<std::string, animation_rects> monster_dict;

    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        auto frames = import_tilemap_rects(cols, rows, entryPath.c_str());
        monster_dict[filename][ANIMATION_IDLE] = frames[0];
        monster_dict[filename][ANIMATION_ATTACK] = frames[1];
    }

    return monster_dict;
}

AttackAnimationRects AttackImporter(const int cols, const int rows, const char *path)
{
    AttackAnimationRects result;

    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        auto frames = import_tilemap_rects(cols, rows, entryPath.c_str());
        result[ATTACK_ANIMATION_NAMES[filename]] = frames[0];
    }

    return result;
}

std::map<std::string, tilerect_face> all_character_import(const char *path)
{
    std::map<std::string, tilerect_face> new_dict = {};

    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        if (dirEntry.is_regular_file())
        {
            auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
            new_dict[filename] = CharacterImporter(4, 4, entryPath.c_str());
        }
    }

    return new_dict;
}

bool CheckConnections(
        const float radius, const Entity *entity, const Entity *target, float tolerance)
{
    const Vector2 relation =
            Vector2Subtract(GetRectCenter(target->rect), GetRectCenter(entity->rect));
    // player is close to target
    if (Vector2Length(relation) < radius)
    {
        // if player is facing left, player is on the right side of target,
        // and in same horizontal plane (not above or below target.y)
        if ((entity->facing_direction == LEFT || entity->facing_direction == LEFT_IDLE) &&
            relation.x < 0 && abs(relation.y) < tolerance)
        {
            return true;
        }
        if ((entity->facing_direction == RIGHT || entity->facing_direction == RIGHT_IDLE) &&
            relation.x > 0 && abs(relation.y) < tolerance)
        {
            return true;
        }
        if ((entity->facing_direction == UP || entity->facing_direction == UP_IDLE) &&
            relation.y < 0 && abs(relation.x) < tolerance)
        {
            return true;
        }
        if ((entity->facing_direction == DOWN || entity->facing_direction == DOWN_IDLE) &&
            relation.y > 0 && abs(relation.x) < tolerance)
        {
            return true;
        }
    }
    return false;
}

std::map<std::string, tmx_map *> tmx_importer(const char *path)
{
    std::map<std::string, tmx_map *> maps;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        maps[filename] = LoadTMX(entryPath.c_str());
    }
    return maps;
}

std::map<std::string, Texture2D>
OutlineCreator(const std::map<std::string, Texture2D> &texture_dict, const float width)
{
    std::map<std::string, Texture2D> outline_texture_dict;
    for (auto &[monster, monster_texture]: texture_dict)
    {
        Image image = LoadImageFromTexture(monster_texture);
        Image mask = ImageFromChannel(image, 3);
        ImageAlphaMask(&mask, mask);

        Image new_image = GenImageColor(image.width, image.height, {0});
        // copy mask in all 8 directions to create an Outline effect
        // Left
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {-width, 0, (float) image.width, (float) image.height}, COLORS["blue"]);
        // Right
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {width, 0, (float) image.width, (float) image.height}, COLORS["blue"]);
        // TopLeft
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {-width, -width, (float) image.width, (float) image.height}, COLORS["blue"]);
        // TopMid
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {0, -width, (float) image.width, (float) image.height}, COLORS["blue"]);
        // TopRight
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {width, -width, (float) image.width, (float) image.height}, COLORS["blue"]);
        // BottomLeft
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {-width, width, (float) image.width, (float) image.height}, COLORS["blue"]);
        // BottomMid
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {0, width, (float) image.width, (float) image.height}, COLORS["blue"]);
        // BottomRight
        ImageDraw(
                &new_image, mask, {0, 0, (float) image.width, (float) image.height},
                {width, width, (float) image.width, (float) image.height}, COLORS["blue"]);

        outline_texture_dict[monster] = LoadTextureFromImage(new_image);

        UnloadImage(new_image);
        UnloadImage(image);
        UnloadImage(mask);
    }
    return outline_texture_dict;
}

std::vector<std::string> split(const std::string &s, const char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

std::map<std::string, Music> MusicsImporter(const char *path)
{
    std::map<std::string, Music> musics;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        musics[filename] = LoadMusicStream(entryPath.c_str());
    }
    return musics;
}

std::map<std::string, Sound> SoundsImporter(const char *path)
{
    std::map<std::string, Sound> sounds;
    for (const auto &dirEntry: recursive_directory_iterator(path))
    {
        auto filename = dirEntry.path().stem().string();
        auto entryPath = dirEntry.path().string();
        sounds[filename] = LoadSound(entryPath.c_str());
    }
    return sounds;
}
