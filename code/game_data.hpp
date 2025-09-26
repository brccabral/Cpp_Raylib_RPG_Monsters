#pragma once
#include <string>
#include <vector>
#include <map>

struct TrainerDialog
{
    std::vector<std::string> default_;
    std::vector<std::string> defeated;
};

struct TrainerMonster
{
    std::string name;
    int level;
};

struct CharacterData
{
    std::vector<TrainerMonster> monsters;
    TrainerDialog dialog;
    std::vector<std::string> directions;
    bool look_around;
    bool defeated;
    std::string biome;
    int radius;
};

enum Attack
{
    ATTACK_NONE = 0,
    ATTACK_BURN,
    ATTACK_HEAL,
    ATTACK_BATTLECRY,
    ATTACK_SPARK,
    ATTACK_SCRATCH,
    ATTACK_SPLASH,
    ATTACK_FIRE,
    ATTACK_EXPLOSTION,
    ATTACK_ANNIHILATE,
    ATTACK_ICE
};

enum AttackAnimation
{
    ATTACKANIMATION_FIRE = 0,
    ATTACKANIMATION_GREEN,
    ATTACKANIMATION_SCRATCH,
    ATTACKANIMATION_SPLASH,
    ATTACKANIMATION_EXPLOSION,
    ATTACKANIMATION_ICE
};

enum ElementType
{
    ELEMENT_FIRE = 0,
    ELEMENT_PLANT,
    ELEMENT_WATER,
    ELEMENT_NORMAL
};

struct MonsterData
{
    std::string name;
    ElementType element;
    std::map<std::string, float> stats;
    std::vector<std::pair<int, Attack>> abilities;
    std::pair<std::string, int> evolve;
};

enum SelectionSide
{
    PLAYER = 0,
    OPPONENT
};

struct AttackData
{
    std::string name;
    // PLAYER - attack same team (healing/defense) | OPPONENT - attack the other team
    SelectionSide target;
    float amount;
    int cost;
    ElementType element;
    AttackAnimation animation;
};

class GameData
{
public:

    static GameData &GetInstance()
    {
        if (!instance)
        {
            instance = new GameData();
        }
        return *instance;
    }

    static void Destroy()
    {
        delete instance;
    }

    CharacterData o1 = {
            {{"Jacana", 14}, {"Cleaf", 15}}, //
            {{"Hey, how are you?", "Oh, so you want to fight?", "FIGHT!"},
             {"You are very strong!", "Let's fight again sometime?"}}, //
            {"down"},
            true,
            false,
            "forest",
            0};

    CharacterData o2 = {
            {{"Atrox", 14}, {"Pouch", 15}, {"Draem", 13}, {"Cindrill", 13}}, //
            {{"I don't like sand", "It's coarse and rough", "oh god, fight"},
             {"May the force be with you"}}, //
            {"left", "down"},
            false,
            false,
            "sand",
            0};

    CharacterData o3 = {
            {{"Atrox", 14}, {"Pouch", 15}, {"Draem", 13}, {"Cindrill", 13}}, //
            {{"I love skating!", "FIGHT!"}, {"Good luck with the boss", "It's so cold in here"}}, //
            {"left", "right", "up", "down"},
            true,
            false,
            "sand",
            0};

    CharacterData o4 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            {{"I love skating!", "FIGHT!"}, {"Good luck with the boss", "It's so cold in here"}}, //
            {"right"},
            true,
            false,
            "forest",
            0};

    CharacterData o5 = {
            {{"Plumette", 20}, {"Ivieron", 22}, {"Atrox", 24}, {"Pouch", 19}}, //
            {{"So you want to challenge the big ones", "This will be fun!"},
             {"I hope the lawyers will never spot you", "<3"}}, //
            {"up", "right"},
            true,
            false,
            "forest",
            0};

    CharacterData o6 = {
            {{"Finsta", 15}, {"Finsta", 15}, {"Finsta", 15}}, //
            {{"I love skating!", "FIGHT!"}, {"Good luck with the boss", "It's so cold in here"}}, //
            {"down"},
            false,
            false,
            "sand",
            0};

    CharacterData o7 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            {{"There are no bugs in the snow!"},
             {"Maybe I should check a vulcano...", "It's so cold in here"}}, //
            {"right"},
            false,
            false,
            "ice",
            0};

    CharacterData p1 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
            {"right"},
            false,
            false,
            "forest",
            0};

    CharacterData p2 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
            {"right"},
            false,
            false,
            "forest",
            0};

    CharacterData p3 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
            {"right"},
            false,
            false,
            "forest",
            0};

    CharacterData p4 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
            {"right"},
            false,
            false,
            "forest",
            0};

    CharacterData px = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Atrox", 24}, {"Finiette", 30}}, //
            {{"I love trees", "and fights"}, {"Good luck with the boss!"}}, //
            {"right"},
            false,
            false,
            "forest",
            0};

    CharacterData w1 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            {{"It's so cold in here", "maybe a fight will warm me up"},
             {"Good luck with the boss!"}}, //
            {"left"},
            true,
            false,
            "ice",
            0};

    CharacterData w2 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            {{"It's so cold in here", "maybe a fight will warm me up"},
             {"Good luck with the boss!"}}, //
            {"right"},
            true,
            false,
            "ice",
            0};

    CharacterData w3 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            {{"It's so cold in here", "maybe a fight will warm me up"},
             {"Good luck with the boss!"}}, //
            {"right"},
            true,
            false,
            "ice",
            0};

    CharacterData w4 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            {{"It's so cold in here", "maybe a fight will warm me up"},
             {"Good luck with the boss!"}}, //
            {"left"},
            true,
            false,
            "ice",
            0};

    CharacterData w5 = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            {{"It's so cold in here", "maybe a fight will warm me up"},
             {"Good luck with the boss!"}}, //
            {"right"},
            true,
            false,
            "ice",
            0};

    CharacterData wx = {
            {{"Friolera", 25}, {"Gulfin", 20}, {"Draem", 24}, {"Finiette", 30}}, //
            {{"I hope you brought rations", "This will be a long journey"}, {"Congratultion!"}}, //
            {"down"},
            false,
            false,
            "ice",
            0};

    CharacterData f1 = {
            {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
            {"right"},
            true,
            false,
            "sand",
            0};

    CharacterData f2 = {
            {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
            {"right", "left"},
            false,
            false,
            "sand",
            0};

    CharacterData f3 = {
            {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
            {"right", "left"},
            true,
            false,
            "sand",
            0};

    CharacterData f4 = {
            {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
            {"up", "right"},
            true,
            false,
            "sand",
            0};

    CharacterData f5 = {
            {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
            {"left"},
            true,
            false,
            "sand",
            0};

    CharacterData f6 = {
            {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            {{"This place feels kinda warm...", "fight!"}, {"Congratultion!"}}, //
            {"right"},
            true,
            false,
            "sand",
            0};

    CharacterData fx = {
            {{"Cindrill", 15}, {"Jacana", 20}, {"Draem", 24}, {"Atrox", 30}}, //
            {{"Time to bring the heat", "fight!"}, {"Congratultion!"}}, //
            {"down"},
            false,
            false,
            "sand",
            0};

    CharacterData Nurse = {
            {}, //
            {{"Welcome to the hospital", "Your monsters have been healed"}, {}}, //
            {"down"},
            false,
            false,
            "None",
            0};


    std::map<std::string, CharacterData> TRAINER_DATA = {
            {"o1", o1}, //
            {"o2", o2}, //
            {"o3", o3}, //
            {"o4", o4}, //
            {"o5", o5}, //
            {"o6", o6}, //
            {"o7", o7}, //
            {"p1", p1}, //
            {"p2", p2}, //
            {"p3", p3}, //
            {"p4", p4}, //
            {"px", px}, //
            {"w1", w1}, //
            {"w2", w2}, //
            {"w3", w3}, //
            {"w4", w4}, //
            {"w5", w5}, //
            {"wx", wx}, //
            {"f1", f1}, //
            {"f2", f2}, //
            {"f3", f3}, //
            {"f4", f4}, //
            {"f5", f5}, //
            {"f6", f6}, //
            {"fx", fx}, //
            {"Nurse", Nurse}, //
    };

    std::map<ElementType, std::string> NAMES_ELEMENT_TYPES{
            {ELEMENT_FIRE, "fire"},
            {ELEMENT_PLANT, "plant"},
            {ELEMENT_WATER, "water"},
            {ELEMENT_NORMAL, "normal"}};

    std::map<std::string, AttackAnimation> ATTACK_ANIMATION_NAMES = {
            {"fire", ATTACKANIMATION_FIRE}, {"green", ATTACKANIMATION_GREEN},
            {"scratch", ATTACKANIMATION_SCRATCH}, {"splash", ATTACKANIMATION_SPLASH},
            {"explosion", ATTACKANIMATION_EXPLOSION}, {"ice", ATTACKANIMATION_ICE},
    };

    std::map<AttackAnimation, std::string> NAMES_ATTACK_ANIMATION = {
            {ATTACKANIMATION_FIRE, "fire"}, {ATTACKANIMATION_GREEN, "green"},
            {ATTACKANIMATION_SCRATCH, "scratch"}, {ATTACKANIMATION_SPLASH, "splash"},
            {ATTACKANIMATION_EXPLOSION, "explosion"}, {ATTACKANIMATION_ICE, "ice"},
    };

    MonsterData Plumette = {
            "Plumette",
            ELEMENT_PLANT,
            {
                    {"max_health", 15.0f}, //
                    {"max_energy", 17.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.0f}, //
                    {"speed", 1.0f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}},
            {"Ivieron", 15} //
    };
    MonsterData Ivieron = {
            "Ivieron",
            ELEMENT_PLANT,
            {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 5.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.2f}, //
                    {"speed", 1.2f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}},
            {"Pluma", 32} //
    };
    MonsterData Pluma = {
            "Pluma",
            ELEMENT_PLANT,
            {
                    {"max_health", 23.0f}, //
                    {"max_energy", 26.0f}, //
                    {"attack", 6.0f}, //
                    {"defense", 12.0f}, //
                    {"recovery", 1.8f}, //
                    {"speed", 1.8f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}},
            {} //
    };
    MonsterData Sparchu = {
            "Sparchu",
            ELEMENT_FIRE,
            {
                    {"max_health", 15.0f}, //
                    {"max_energy", 7.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.1f}, //
                    {"speed", 1.0f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_FIRE}, //
             {15, ATTACK_BATTLECRY}, //
             {26, ATTACK_EXPLOSTION}},
            {"Cindrill", 15} //
    };
    MonsterData Cindrill = {
            "Cindrill",
            ELEMENT_FIRE,
            {
                    {"max_health", 18.0f}, //
                    {"max_energy", 10.0f}, //
                    {"attack", 3.5f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.2f}, //
                    {"speed", 1.1f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_FIRE}, //
             {15, ATTACK_BATTLECRY}, //
             {26, ATTACK_EXPLOSTION}},
            {"Charmadillo", 33} //
    };
    MonsterData Charmadillo = {
            "Charmadillo",
            ELEMENT_FIRE,
            {
                    {"max_health", 29.0f}, //
                    {"max_energy", 12.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 17.0f}, //
                    {"recovery", 1.35f}, //
                    {"speed", 1.1f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_FIRE}, //
             {15, ATTACK_BATTLECRY}, //
             {26, ATTACK_EXPLOSTION}, //
             {45, ATTACK_ANNIHILATE}},
            {} //
    };
    MonsterData Finsta = {
            "Finsta",
            ELEMENT_WATER,
            {
                    {"max_health", 13.0f}, //
                    {"max_energy", 17.0f}, //
                    {"attack", 2.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.5f}, //
                    {"speed", 1.8f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}, //
             {15, ATTACK_SPLASH}, //
             {20, ATTACK_ICE}, //
             {25, ATTACK_HEAL}},
            {"Gulfin", 34} //
    };
    MonsterData Gulfin = {
            "Gulfin",
            ELEMENT_WATER,
            {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.8f}, //
                    {"speed", 2.0f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}, //
             {15, ATTACK_SPLASH}, //
             {20, ATTACK_ICE}, //
             {25, ATTACK_HEAL}},
            {"Finiette", 45} //
    };
    MonsterData Finiette = {
            "Finiette",
            ELEMENT_WATER,
            {
                    {"max_health", 27.0f}, //
                    {"max_energy", 23.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 17.0f}, //
                    {"recovery", 2.0f}, //
                    {"speed", 2.5f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}, //
             {15, ATTACK_SPLASH}, //
             {20, ATTACK_ICE}, //
             {25, ATTACK_HEAL}},
            {} //
    };
    MonsterData Atrox = {
            "Atrox",
            ELEMENT_FIRE,
            {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.3f}, //
                    {"speed", 1.9f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}, //
             {30, ATTACK_FIRE}},
            {} //
    };
    MonsterData Pouch = {
            "Pouch",
            ELEMENT_PLANT,
            {
                    {"max_health", 23.0f}, //
                    {"max_energy", 25.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 12.0f}, //
                    {"recovery", 1.0f}, //
                    {"speed", 1.5f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}, //
             {25, ATTACK_HEAL}},
            {} //
    };
    MonsterData Draem = {
            "Draem",
            ELEMENT_PLANT,
            {
                    {"max_health", 23.0f}, //
                    {"max_energy", 25.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 12.0f}, //
                    {"recovery", 1.2f}, //
                    {"speed", 1.4f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_HEAL}, //
             {20, ATTACK_EXPLOSTION}, //
             {25, ATTACK_SPLASH}},
            {} //
    };
    MonsterData Larvea = {
            "Larvea",
            ELEMENT_PLANT,
            {
                    {"max_health", 15.0f}, //
                    {"max_energy", 17.0f}, //
                    {"attack", 1.0f}, //
                    {"defense", 8.0f}, //
                    {"recovery", 1.0f}, //
                    {"speed", 1.0f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}},
            {"Cleaf", 4} //
    };
    MonsterData Cleaf = {
            "Cleaf",
            ELEMENT_PLANT,
            {
                    {"max_health", 18.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 1.7f}, //
                    {"speed", 1.6f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_HEAL}},
            {} //
    };
    MonsterData Jacana = {
            "Jacana",
            ELEMENT_FIRE,
            {
                    {"max_health", 12.0f}, //
                    {"max_energy", 19.0f}, //
                    {"attack", 3.0f}, //
                    {"defense", 10.0f}, //
                    {"recovery", 2.1f}, //
                    {"speed", 2.6f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}, //
             {15, ATTACK_BURN}, //
             {20, ATTACK_EXPLOSTION}, //
             {25, ATTACK_HEAL}},
            {} //
    };
    MonsterData Friolera = {
            "Friolera",
            ELEMENT_WATER,
            {
                    {"max_health", 13.0f}, //
                    {"max_energy", 20.0f}, //
                    {"attack", 4.0f}, //
                    {"defense", 6.0f}, //
                    {"recovery", 1.3f}, //
                    {"speed", 2.0f}, //
            },
            {{0, ATTACK_SCRATCH}, //
             {5, ATTACK_SPARK}, //
             {15, ATTACK_SPLASH}, //
             {20, ATTACK_ICE}, //
             {25, ATTACK_HEAL}},
            {} //
    };

    std::map<std::string, MonsterData> MONSTER_DATA = {
            {"Plumette", Plumette}, //
            {"Ivieron", Ivieron}, //
            {"Pluma", Pluma}, //
            {"Sparchu", Sparchu}, //
            {"Cindrill", Cindrill}, //
            {"Charmadillo", Charmadillo}, //
            {"Finsta", Finsta}, //
            {"Gulfin", Gulfin}, //
            {"Finiette", Finiette}, //
            {"Atrox", Atrox}, //
            {"Pouch", Pouch}, //
            {"Draem", Draem}, //
            {"Larvea", Larvea}, //
            {"Cleaf", Cleaf}, //
            {"Jacana", Jacana}, //
            {"Friolera", Friolera}, //
    };

    std::map<Attack, AttackData> ATTACK_DATA = {
            {ATTACK_BURN, {"burn", OPPONENT, 2.0f, 15, ELEMENT_FIRE, ATTACKANIMATION_FIRE}}, //
            {ATTACK_HEAL, {"heal", PLAYER, -1.2f, 600, ELEMENT_PLANT, ATTACKANIMATION_GREEN}}, //
            {ATTACK_BATTLECRY,
             {"battlecry", PLAYER, -1.4f, 20, ELEMENT_NORMAL, ATTACKANIMATION_GREEN}}, //
            {ATTACK_SPARK, {"spark", OPPONENT, 1.1f, 20, ELEMENT_FIRE, ATTACKANIMATION_FIRE}}, //
            {ATTACK_SCRATCH,
             {"scratch", OPPONENT, 1.2f, 20, ELEMENT_NORMAL, ATTACKANIMATION_SCRATCH}}, //
            {ATTACK_SPLASH, {"splash", OPPONENT, 2.0f, 15, ELEMENT_WATER, ATTACKANIMATION_SPLASH}},
            //
            {ATTACK_FIRE, {"fire", OPPONENT, 2.0f, 15, ELEMENT_FIRE, ATTACKANIMATION_FIRE}}, //
            {ATTACK_EXPLOSTION,
             {"explosion", OPPONENT, 2.0f, 90, ELEMENT_FIRE, ATTACKANIMATION_EXPLOSION}}, //
            {ATTACK_ANNIHILATE,
             {"annihilate", OPPONENT, 3.0f, 30, ELEMENT_FIRE, ATTACKANIMATION_EXPLOSION}}, //
            {ATTACK_ICE, {"ice", OPPONENT, 2.0f, 15, ELEMENT_WATER, ATTACKANIMATION_ICE}}, //
    };

private:

    GameData() = default;
    ~GameData() = default;

    static GameData *instance;
};
