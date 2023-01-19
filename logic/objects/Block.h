#pragma once

#include <random>

#include "Utility.h"

class Block : public BaseObject {
 public:
  bool is_solid = false;
  bool is_destructible = false;
  virtual ~Block() = default;
};

class Wall : public Block {
 public:
  Wall() {
    pic = L"█";
    is_solid = true;
  }

  std::wstring Description() override { return L"Wall"; }
};

class WeakWall : public Block {
 public:
  WeakWall() {
    pic = L"#";
    is_destructible = true;
    is_solid = true;
  }

  std::wstring Description() override { return L"Weak Wall"; }
};

class Door : public Block {
 public:
  Door() {
    pic = L"⋂";
    is_solid = true;
  }

  void OnCollide(const std::shared_ptr<Entity> &entity) override {
    pic = L"⁻";
    is_solid = false;
  }

  std::wstring Description() override {
    if (is_solid)
      return L"Door";
    else
      return L"Opened Door";
  }
};

class Water : public Block {
 public:
  Water() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::wstring> blocks = {L"☰", L"☲", L"☱", L"☴",
                                        L"☵", L"☶", L"☳", L"☷"};

    std::uniform_int_distribution<> dist(0, blocks.size() - 1);
    pic = blocks[dist(gen)];
    is_solid = true;
  }

  std::wstring Description() override { return L"Water"; }
};
