#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "Drawable.hpp"
#include <smk/Sprite.hpp>

namespace smk {

class Shape : public Sprite {
 public:
  static Shape Rectangle(float left,
                         float top,
                         float right,
                         float bottom,
                         glm::vec4 color,
                         float thickness = 0,
                         glm::vec4 borderColor = {0.0, 0.0, 0.0, 1.0});
  static Shape Line(float x1,
                    float y1,
                    float x2,
                    float y2,
                    float thick,
                    glm::vec4 color,
                    float thickness = 0,
                    glm::vec4 borderColor = {0.0, 0.0, 0.0, 1.0});
  static Shape Circle(float x, float y, float radius, glm::vec4 color);
};

} // namespace smk

#endif /* end of include guard: SHAPE_HPP */
