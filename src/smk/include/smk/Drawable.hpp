#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <smk/RenderState.hpp>

namespace smk {
  struct Texture;
  class Screen;
  class VertexArray;

class Drawable {
 public:
  virtual ~Drawable() = default;

  // Transformation.
  void Move(float move_x, float move_y);
  void Rotate(float rotation);
  void SetCenter(float position_x, float position_y);
  void SetPosition(float position_x, float position_y);
  void SetPosition(glm::vec2 position);
  void SetRotation(float rotation);
  void SetScale(float scale_x, float scale_y);
  void SetScaleX(float scale_x);
  void SetScaleY(float scale_y);
  glm::mat4 Transformation() const;

  // Color
  void SetColor(const glm::vec4& color);
  const glm::vec4& color() const { return color_; }

  // Texture
  void SetTexture(const Texture& texture);
  const Texture* texture() const { return texture_;}

  // BlendMode
  void SetBlendMode(const BlendMode&);
  const BlendMode& blend_mode() const { return blend_mode_; }

  virtual void Draw(Screen& screen, RenderState state) const = 0;

 private:
  float rotation_ = 0.f;
  glm::vec2 position_;
  float center_x_ = 0.f;
  float center_y_ = 0.f;
  float scale_x_ = 1.f;
  float scale_y_ = 1.f;
  glm::vec4 color_ = {1.0, 1.0, 1.0, 1.0};
  const Texture* texture_ = nullptr;
  BlendMode blend_mode_;
};

} // namespace smk

#endif /* end of include guard: DRAWABLE_HPP */
