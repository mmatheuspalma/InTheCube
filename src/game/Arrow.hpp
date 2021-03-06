#ifndef GAME_ARROW_HPP
#define GAME_ARROW_HPP

#include <glm/glm.hpp>
#include <smk/Sprite.hpp>
#include "game/Forme.hpp"

namespace smk {
class Window;
}  // namespace smk

class Arrow {
 public:
  Arrow(glm::vec2 position, glm::vec2 speed);
  void Step();
  void Draw(smk::Window& window);

  bool damage;

  glm::vec2 position;
  glm::vec2 speed;
  int alpha;
 private:
  smk::Sprite sprite;
};

#endif /* GAME_ARROW_HPP */
