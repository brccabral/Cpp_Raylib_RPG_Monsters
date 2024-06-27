#pragma once
#include "raylib-tmx.h"
#include "entities.h"

RLAPI std::vector<Texture2D> ImportFolder(const char *path);

RLAPI std::map<std::string, Texture2D> ImportNamedFolder(const char *path);

typedef std::map<std::string, std::vector<RectangleU>> tilerect_name;
typedef std::map<AnimationState, std::vector<RectangleU>> animation_rects;
typedef std::map<AttackAnimation, std::vector<RectangleU>> AttackAnimationRects;
typedef std::map<FacingDirection, std::vector<RectangleU>> tilerect_face;

RLAPI std::map<std::string, tilerect_name> coast_rects();

RLAPI std::vector<std::vector<RectangleU>>
import_tilemap_rects(int cols, int rows, const char *path);

RLAPI tilerect_face CharacterImporter(int cols, int rows, const char *path);

RLAPI std::map<std::string, animation_rects> MonsterImporter(int cols, int rows, const char *path);

RLAPI AttackAnimationRects AttackImporter(int cols, int rows, const char *path);

RLAPI std::map<std::string, tilerect_face> all_character_import(const char *path);

RLAPI bool
CheckConnections(float radius, const Entity *entity, const Entity *target, float tolerance = 30);

RLAPI std::map<std::string, tmx_map *> tmx_importer(const char *path);

RLAPI std::map<std::string, Texture2D>
OutlineCreator(const std::map<std::string, Texture2D> &texture_dict, float width);

std::vector<std::string> split(const std::string &s, char delim);
