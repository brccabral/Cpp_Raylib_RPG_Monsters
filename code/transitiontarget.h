#pragma once
#include <string>
#include "battle.h"

enum TransitionTargetType
{
    TRANSITIONTARGET_NONE = 0,
    TRANSITIONTARGET_MAP,
    TRANSITIONTARGET_LEVEL2BATTLE,
    TRANSITIONTARGET_BATTLE2LEVEL
};

class TransitionTarget
{
public:

    explicit TransitionTarget(TransitionTargetType target_type);

    std::string map_name{};
    std::string start_position{};
    Battle *battle = nullptr;
    TransitionTargetType target_type{};
};
