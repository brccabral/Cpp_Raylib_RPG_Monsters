#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "settings.hpp"


std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>>
CoastImporter(const char *file, int rows, int cols);

std::unordered_map<std::string, std::unordered_map<std::string, rg::Frames>> AllCharacterImport(
        const char *path);

std::unordered_map<std::string, rg::Frames> CharacterImporter(int rows, int cols, const char *file);

bool CheckConnections(
        float radius, const Entity *entity, const Entity *target,
        float tolerance = 30);

std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>>
MonsterImporter(int cols, int rows, const char *path);

std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> OutlineCreator(
        const std::unordered_map<std::string, std::unordered_map<AnimationState, rg::Frames>> &
        monster_frames,
        float width);

std::unordered_map<AttackAnimation, rg::Frames> AttackImporter(const char *path);

std::unordered_map<std::string, rg::mixer::Sound> AudioImporter(const char *path);
