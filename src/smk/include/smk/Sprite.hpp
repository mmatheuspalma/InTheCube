#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "Drawable.hpp"
#include <smk/Texture.hpp>

namespace smk {

class Sprite : public Drawable {
 public:
  void Draw(const glm::mat4& view) const;

 public:
  class Context;
  friend Context;
};

}  // namespace smk

#endif /* end of include guard: SPRITE_HPP */
