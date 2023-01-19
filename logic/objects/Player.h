#pragma once

#include "Config.h"
#include "TimedObjects.h"
#include "Utility.h"
#include "Weapon.h"

class Player : public Entity {
 public:
  Sword sword = Sword();
  Bow bow = Bow();
  int damage_default{};
  int arrows{};
  int bombs{};

  Player() {
    pic = L"@";
    is_player = true;
  }

  void Configure(const Config& config) override {
    SetEntity(config.player_hp, config.player_damage);
    damage_default = damage;
    arrows = config.player_arrows;
    bombs = config.player_bombs;
  }

  void CreateArrow(Direction dir_,
                   std::shared_ptr<std::vector<std::shared_ptr<TimedObject>>>&
                       timed_objects) {
    if (arrows <= 0) return;
    --arrows;

    Arrow arrow(dir_, pos.ToPos(dir_), damage_default * bow.multiplier);
    arrow.max_moves++;
    arrow.is_players = true;
    timed_objects->push_back(std::make_shared<Arrow>(arrow));
  }

  void CreateBomb(
      std::shared_ptr<std::vector<std::shared_ptr<TimedObject>>>& timed_objects,
      const Config& config_) {
    if (bombs <= 0) return;
    --bombs;

    Bomb bomb(pos, damage_default * 4, config_);
    bomb.is_players = true;
    timed_objects->push_back(std::make_shared<Bomb>(bomb));
  }

  void Update() {
    damage = damage_default * sword.multiplier;
    sword.MakeMove();
    bow.MakeMove();
  }

  std::wstring Description() override { return L"Player"; }
};

class Princess : public BaseObject {
 public:
  Princess() { pic = L"♛"; }

  std::wstring Description() override { return L"Princess"; }
};
