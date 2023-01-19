#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Block.h"
#include "Enemy.h"
#include "Object.h"
#include "Player.h"
#include "Utility.h"

class Collider {
 public:
  std::shared_ptr<std::vector<std::wstring>> story;

  Action Collide(const std::shared_ptr<Player>& player, const Pos& to_pos,
                 std::vector<std::shared_ptr<Enemy>>& objects) {
    auto object = FindByPos(objects, to_pos);
    if (object != objects.end()) {
      object->get()->TakeDamage(player);
      story->emplace_back(L"Player dealt " + std::to_wstring(player->damage) +
                          L" damage");
      return STAY;
    }
    return MOVE;
  }

  Action Collide(const std::shared_ptr<Player>& player, const Pos& to_pos,
                 std::vector<std::shared_ptr<Block>>& objects) {
    auto object = FindByPos(objects, to_pos);
    if (object != objects.end()) {
      if (!object->get()->is_solid) return MOVE;
      object->get()->OnCollide(player);
      return STAY;
    }
    return MOVE;
  }

  Action Collide(const std::shared_ptr<Player>& player, const Pos& to_pos,
                 std::vector<std::shared_ptr<Object>>& objects) {
    auto object = FindByPos(objects, to_pos);
    if (object != objects.end()) {
      object->get()->OnCollide(player);
      story->emplace_back(L"Player took " + object->get()->Description());
    }
    return MOVE;
  }
};