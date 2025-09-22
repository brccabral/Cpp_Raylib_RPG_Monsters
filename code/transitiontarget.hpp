#pragma once
#include <string>
#include "battle.hpp"


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

    TransitionTarget() = default;
    explicit TransitionTarget(TransitionTargetType target_type);

    std::string map_name{};
    std::string start_position{};
    Battle battle{};
    TransitionTargetType target_type{};
};
