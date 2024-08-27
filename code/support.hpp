#pragma once
#include <rygame.hpp>
#include "entities.hpp"
#include "settings.hpp"


std::map<std::string, std::map<std::string, rg::Frames_Ptr>>
CoastImporter(const char *file, int rows, int cols);

std::map<std::string, std::map<std::string, rg::Frames_Ptr>> AllCharacterImport(const char *path);

std::map<std::string, rg::Frames_Ptr> CharacterImporter(int rows, int cols, const char *file);

bool CheckConnections(
        float radius, const std::shared_ptr<Entity> &entity, const std::shared_ptr<Entity> &target,
        float tolerance = 30);

std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>>
MonsterImporter(int cols, int rows, const char *path);

std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> OutlineCreator(
        const std::map<std::string, std::map<AnimationState, rg::Frames_Ptr>> &monster_frames,
        float width);

std::map<AttackAnimation, rg::Frames_Ptr> AttackImporter(const char *path);

std::map<std::string, std::shared_ptr<rg::mixer::Sound>> AudioImporter(const char *path);
