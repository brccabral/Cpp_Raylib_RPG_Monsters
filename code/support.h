#pragma once
#include "entities.h"


#include <rygame.h>

std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
CoastImporter(const char *file, int rows, int cols);

std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
AllCharacterImport(const char *path);

std::map<std::string, std::shared_ptr<rg::Frames>>
CharacterImporter(int rows, int cols, const char *file);

bool CheckConnections(
        float radius, const std::shared_ptr<Entity> &entity, const std::shared_ptr<Entity> &target,
        float tolerance = 30);

std::map<std::string, std::map<std::string, std::shared_ptr<rg::Frames>>>
MonsterImporter(int cols, int rows, const char *path);
