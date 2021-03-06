#ifndef GAME_HERO_HPP
#define GAME_HERO_HPP

#include "game/Collision.hpp"
#include "game/Resource.hpp"
#include <smk/Sprite.hpp>

namespace smk {
class Window;
}  // namespace smk

class Hero {
 public:
  Rectangle geometry;
  smk::Sprite sprite;
  float x = 0.f;
  float y = 0.f;
  float xspeed = 0.f;
  float yspeed = 0.f;
  int life;
  bool sens = true;

  bool in_laser = false;

  //Hero();
  Hero(float x, float y);
  void SetPosition(float x, float y);
  void UpdateGeometry();

  void Draw(smk::Window& window, bool selected);
};

#endif /* GAME_HERO_HPP */
