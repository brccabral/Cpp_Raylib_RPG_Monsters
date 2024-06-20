#include <cstring>
#include <utility>
#include <algorithm>
#include "sprite.h"
#include "dialogsprite.h"
#include "entities.h"
#include "raylib_utils.h"
#include "settings.h"


SimpleSprite::SimpleSprite(const std::vector<SpriteGroup *> &sprite_groups)
{
    for (auto *sprite_group: sprite_groups)
    {
        groups.push_back(sprite_group);
        sprite_group->sprites.push_back(this);
    }
}

// the TMX images are unloaded in ~Game()::UnloadTMX
// if the sub class has its own render/texture, unload it there
SimpleSprite::~SimpleSprite() = default;

void SimpleSprite::Draw(const Vector2 offset) const
{
    const Vector2 pos = Vector2Add(rect.pos, offset);

    if (image.texture)
    {
        DrawTextureRec(*image.texture, image.rect.rectangle, pos, WHITE);
    }
    else
    {
        DrawRectangle(pos.x, pos.y, rect.width, rect.height, BLACK);
    }
}

void SimpleSprite::LeaveOtherGroups(const SpriteGroup *sprite_group)
{
    for (const auto group: groups)
    {
        if (group != sprite_group)
        {
            group->sprites.erase(
                    std::remove(group->sprites.begin(), group->sprites.end(), this),
                    group->sprites.end());
        }
    }
}

void SimpleSprite::FlipH()
{
    image.rect.width = -image.rect.width;
}

Sprite::Sprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs,
        const int z_)
    : SimpleSprite(sgs), z(z_)
{
    image = img;
    rect = {pos.x, pos.y, image.rect.width, image.rect.height};

    y_sort = GetRectCenter(rect).y;
    type = SPRITE;
    hitbox = rect;
}

MonsterPatchSprite::MonsterPatchSprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs,
        std::string bio)
    : Sprite(pos, img, sgs, WORLD_LAYERS["main"]), biome(std::move(bio))
{
    y_sort -= 40;
    // move sand patches to background drawing layer
    if (strcmp(biome.c_str(), "sand") == 0)
    {
        z = WORLD_LAYERS["bg"];
    }
}

BorderSprite::BorderSprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, img, sgs)
{}

TransitionSprite::TransitionSprite(
        const Vector2 pos, const Vector2 size, std::array<std::string, 2> target,
        const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, {{}, {0, 0, size.x, size.y}}, sgs), target(std::move(target))
{}

CollidableSprite::CollidableSprite(
        const Vector2 pos, const TiledTexture &img, const std::vector<SpriteGroup *> &sgs)
    : Sprite(pos, img, sgs)
{
    // smaller hitbox for better view perpective on Y axis
    RectInflate(hitbox, 0, -rect.height * 0.6f);
}

AnimatedSprite::AnimatedSprite(
        const Vector2 position, const std::vector<TiledTexture> &frms,
        const std::vector<SpriteGroup *> &sgs, const int z)
    : Sprite(position, frms[0], sgs, z), frame_index(0), frames(frms)
{}

void AnimatedSprite::Animate(const double deltaTime)
{
    frame_index += ANIMATION_SPEED * deltaTime;
    image = frames[int(frame_index) % frames.size()];
}

void AnimatedSprite::Update(const double deltaTime)
{
    Animate(deltaTime);
}

void AnimatedSprite::FlipH()
{
    for (auto &[texture, rect]: frames)
    {
        rect.width = -rect.width;
    }
}

MonsterSprite::MonsterSprite(
        const Vector2 position, const std::map<std::string, std::vector<TiledTexture>> &frms,
        const std::vector<SpriteGroup *> &sgs, Monster monster, const int index,
        const int pos_index, const std::string &entity)
    : SimpleSprite(sgs), monster(std::move(monster)), index(index), pos_index(pos_index),
      entity(entity), frames(frms)
{
    image = frames[state][int(frame_index)];
    rect = image.rect;
    RectToCenter(rect, position);
    animation_speed = ANIMATION_SPEED + GetRandomUniform(-1, 1);
}

void MonsterSprite::Animate(const double dt)
{
    frame_index += animation_speed * dt;
    image = frames[state][int(frame_index) % frames[state].size()];
}

void MonsterSprite::Update(const double dt)
{
    Animate(dt);
}

void MonsterSprite::FlipH()
{
    for (auto &[state, tiles]: frames)
    {
        for (auto &[texture, rect]: tiles)
        {
            rect.width = -rect.width;
        }
    }
}

MonsterNameSprite::MonsterNameSprite(
        const Vector2 pos, MonsterSprite *monster_sprite, const std::vector<SpriteGroup *> &sgs,
        Font font)
    : SimpleSprite(sgs), monster_sprite(monster_sprite)
{
    const auto [text_width, text_height] =
            MeasureTextEx(font, monster_sprite->monster.name.c_str(), font.baseSize, 1);
    int padding = 10;
    const Vector2 render_size = {text_width + 2 * padding, text_height + 2 * padding};

    // need another render to invert the image
    // https://github.com/raysan5/raylib/issues/3803
    // https://github.com/raysan5/raylib/issues/378
    RenderTexture2D inverted = LoadRenderTextureV(render_size);
    Vector2 text_pos =
            GetRectMidTop({0, 0, (float) inverted.texture.width, (float) inverted.texture.height});
    text_pos = Vector2Add(text_pos, {-text_width / 2, text_height / 2});
    BeginTextureModeC(inverted, BLANK);
    DrawRectangle(0, 0, render_size.x, render_size.y, WHITE);
    DrawTextEx(
            font, monster_sprite->monster.name.c_str(), text_pos, font.baseSize, 1,
            COLORS["black"]);
    EndTextureMode();

    render = LoadRenderTextureV(render_size);
    BeginTextureModeC(render, BLANK);
    DrawTexture(inverted.texture, 0, 0, WHITE);
    EndTextureMode();

    image.rect = {Vector2{0.0f, 0.0f}, render_size};
    image.texture = &render.texture;

    rect = image.rect;
    rect.pos = pos;

    UnloadRenderTexture(inverted);
}

MonsterNameSprite::~MonsterNameSprite()
{
    UnloadRenderTexture(render);
}

MonsterLevelSprite::MonsterLevelSprite(
        const std::string &entity, const Vector2 pos, MonsterSprite *monster_sprite,
        const std::vector<SpriteGroup *> &sgs, const Font &font)
    : SimpleSprite(sgs), entity(entity), monster_sprite(monster_sprite), font(font)
{
    constexpr Vector2 render_size = {60, 26};

    RenderTexture2D inverted = LoadRenderTextureV(render_size);
    BeginTextureModeC(inverted, BLANK);
    DrawRectangle(0, 0, 60, 26, WHITE);
    DrawText("Lvl", 0, 0, 20, BLACK);
    EndTextureMode();

    render = LoadRenderTextureV(render_size);
    BeginTextureModeC(render, BLANK);
    DrawTexture(inverted.texture, 0, 0, WHITE);
    EndTextureMode();

    image.rect = {0, 0, render_size};
    image.texture = &render.texture;

    rect = image.rect;
    if (strcmp(entity.c_str(), "player") == 0)
    {
        RectToTopLeft(rect, pos);
    }
    else
    {
        RectToTopRight(rect, pos);
    }

    UnloadRenderTexture(inverted);
}

MonsterLevelSprite::~MonsterLevelSprite()
{
    UnloadRenderTexture(render);
}

void MonsterLevelSprite::Update(double deltaTime)
{
    SimpleSprite::Update(deltaTime);
}

void SpriteGroup::Draw() const
{
    for (const auto *sprite: sprites)
    {
        sprite->Draw({0, 0});
    }
}

void SpriteGroup::Update(const double deltaTime)
{
    for (auto *sprite: sprites)
    {
        sprite->Update(deltaTime);
    }
}

int GetZ(const SimpleSprite *sprite)
{
    int z = 0;

    if (sprite->type == SPRITE)
    {
        z = ((Sprite *) sprite)->z;
    }
    else if (sprite->type == ENTITY)
    {
        z = ((Entity *) sprite)->z;
    }
    else if (sprite->type == DIALOGSPRITE)
    {
        z = ((DialogSprite *) sprite)->z;
    }
    return z;
}

int GetYsort(const SimpleSprite *sprite)
{
    int y = 0;

    if (sprite->type == SPRITE)
    {
        y = ((Sprite *) sprite)->y_sort;
    }
    else if (sprite->type == ENTITY)
    {
        y = ((Entity *) sprite)->y_sort;
    }
    else if (sprite->type == DIALOGSPRITE)
    {
        y = ((DialogSprite *) sprite)->y_sort;
    }
    return y;
}

RectangleU GetHitbox(const SimpleSprite *sprite)
{
    RectangleU rect{};

    if (sprite->type == SPRITE)
    {
        rect = ((Sprite *) sprite)->hitbox;
    }
    else if (sprite->type == ENTITY)
    {
        rect = ((Entity *) sprite)->hitbox;
    }
    return rect;
}

SpriteGroup::~SpriteGroup()
{
    for (auto *sprite: sprites)
    {
        // remove sprite from other groups to avoid double `delete`
        sprite->LeaveOtherGroups(this);
        delete sprite;
    }
}
