#pragma once

class Weapon {
 protected:
  int max_moves_default = 10;
  int multiplier_default = 2;

 public:
  int move{};
  int max_moves{};
  int multiplier = 1;

  Weapon() { max_moves = 0; }

  Weapon(const Weapon& weapon) = default;
  Weapon& operator=(const Weapon& weapon) = default;

  void MakeDefault() {
    max_moves = 0;
    move = 0;
    multiplier = 1;
  }

  void MakeMove() {
    if (max_moves <= 0) return;

    ++move;
    if (move < max_moves) return;

    MakeDefault();
  }

  void Configure(const Config& config) {
    max_moves_default = config.weapon_duration;
    multiplier_default = config.weapon_multiplier;
  }
};

class Sword : public Weapon {
 public:
  Sword() : Weapon(){};

  explicit Sword(const Config& config) {
    Configure(config);
    max_moves = max_moves_default;
    multiplier = multiplier_default;
  }

  explicit Sword(const Weapon& weapon) {
    move = weapon.move;
    max_moves = weapon.max_moves;
    multiplier = weapon.multiplier;
  }
};

class BigSword : public Sword {
 public:
  explicit BigSword(const Config& config) {
    Configure(config);
    max_moves = max_moves_default;
    multiplier = multiplier_default * 2;
  }
};

class Bow : public Weapon {
 public:
  Bow() : Weapon(){};

  explicit Bow(const Config& config) {
    Configure(config);
    max_moves = max_moves_default;
    multiplier = multiplier_default;
  }

  explicit Bow(const Weapon& weapon) {
    move = weapon.move;
    max_moves = weapon.max_moves;
    multiplier = weapon.multiplier;
  }
};

class LongBow : public Bow {
 public:
  explicit LongBow(const Config& config) {
    Configure(config);
    max_moves = max_moves_default;
    multiplier = multiplier_default * 2;
  }
};