#pragma once

#include "Player.h"
#include "Utility.h"

class Object : public BaseObject {
 protected:
  bool is_taken = false;
  Config config;

 public:
  bool IsToDestroy() override { return is_taken; }

  void Configure(const Config& config_) override { config = config_; }
};

class HealthPotion : public Object {
 public:
  HealthPotion() { pic = L"⊕"; }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    if (entity->is_player) {
      entity->health =
          std::min(entity->health + config.health_potion, entity->max_health);
      is_taken = true;
    }
  }

  std::wstring Description() override {
    return L"Health Potion: " + std::to_wstring(config.health_potion) + L" hp";
  }
};

class Quiver : public Object {
 public:
  Quiver() { pic = L"√"; }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    if (entity->is_player) {
      std::dynamic_pointer_cast<Player>(entity)->arrows +=
          config.quiver_capacity;
      is_taken = true;
    }
  }

  std::wstring Description() override {
    return L"Quiver: " + std::to_wstring(config.quiver_capacity) + L" arrows";
  }
};

class SwordWeapon : public Object {
 public:
  SwordWeapon() { pic = L"s"; }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    if (entity->is_player) {
      auto player = std::dynamic_pointer_cast<Player>(entity);
      player->sword = Sword(config);
      is_taken = true;
    }
  }

  std::wstring Description() override {
    return L"Sword: " + std::to_wstring(Sword(config).max_moves) + L" moves, " +
           std::to_wstring(Sword(config).multiplier) + L"x mult";
  }
};

class BigSwordWeapon : public SwordWeapon {
 public:
  BigSwordWeapon() { pic = L"S"; }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    if (entity->is_player) {
      auto player = std::dynamic_pointer_cast<Player>(entity);
      player->sword = BigSword(config);
      is_taken = true;
    }
  }

  std::wstring Description() override {
    return L"Big Sword: " + std::to_wstring(BigSword(config).max_moves) +
           L" moves, " + std::to_wstring(BigSword(config).multiplier) +
           L"x mult";
  }
};

class BowWeapon : public Object {
 public:
  BowWeapon() { pic = L"b"; }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    if (entity->is_player) {
      auto player = std::dynamic_pointer_cast<Player>(entity);
      player->bow = Bow(config);
      player->arrows += config.quiver_capacity / 4;
      is_taken = true;
    }
  }

  std::wstring Description() override {
    return L"Bow: " + std::to_wstring(Bow(config).max_moves) + L" moves, " +
           std::to_wstring(Bow(config).multiplier) + L"x mult";
  }
};

class LongBowWeapon : public BowWeapon {
 public:
  LongBowWeapon() { pic = L"B"; }

  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    if (entity->is_player) {
      auto player = std::dynamic_pointer_cast<Player>(entity);
      player->bow = LongBow(config);
      player->arrows += config.quiver_capacity / 2;
      is_taken = true;
    }
  }

  std::wstring Description() override {
    return L"Long Bow: " + std::to_wstring(LongBow(config).max_moves) +
           L" moves, " + std::to_wstring(LongBow(config).multiplier) +
           L"x mult";
  }
};

class BombObject : public Object {
 public:
  BombObject() { pic = L"⧃"; }
  void OnCollide(const std::shared_ptr<Entity>& entity) override {
    if (entity->is_player) {
      std::dynamic_pointer_cast<Player>(entity)->bombs += 1;
      is_taken = true;
    }
  }

  std::wstring Description() override { return L"Bomb"; }
};