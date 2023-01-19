#pragma once

#include "Utility.h"

class Decoration : public BaseObject {};

class Grass : public Decoration {
 public:
  Grass() { pic = L","; }

  std::wstring Description() override { return L"Grass"; }
};