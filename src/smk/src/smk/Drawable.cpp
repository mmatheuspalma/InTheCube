#include <glm/gtx/transform.hpp>
#include <smk/Drawable.hpp>
#include <smk/Texture.hpp>

namespace smk {

void Drawable::SetRotation(float rotation) {
  rotation_ = rotation;
}

void Drawable::Rotate(float rotation) {
  rotation_ += rotation;
}

void Drawable::SetPosition(float position_x, float position_y) {
  position_x_ = position_x;
  position_y_ = position_y;
}

void Drawable::SetCenter(float center_x, float center_y) {
  center_x_ = center_x;
  center_y_ = center_y;
}

void Drawable::SetScale(float scale_x, float scale_y) {
  scale_x_ = scale_x;
  scale_y_ = scale_y;
}

void Drawable::SetScaleX(float scale_x) {
  scale_x_ = scale_x;
}

void Drawable::SetScaleY(float scale_y) {
  scale_y_ = scale_y;
}

void Drawable::Move(float move_x, float move_y) {
  position_x_ += move_x;
  position_y_ += move_y;
}

glm::mat4 Drawable::transformation() const {
  float size_x = 1.0;
  float size_y = 1.0;
  if (texture_) {
    size_x = texture_->width;
    size_y = texture_->height;
  }
  glm::mat4 ret = glm::mat4(1.0);
  ret = glm::translate(ret, glm::vec3(position_x_, position_y_, 0.0));
  ret = glm::rotate(ret, -rotation_ / 360.f * 2.f * 3.1415f,
                    glm::vec3(0.0, 0.0, 1.0));
  ret = glm::translate(ret, glm::vec3(-center_x_, -center_y_, 0.f));
  ret = glm::scale(ret, glm::vec3(size_x * scale_x_, +size_y * scale_y_, 1.0));
  return ret;
}

void Drawable::SetColor(const glm::vec4& color) {
  color_ = color;
}

void Drawable::SetBlendMode(Blend) {}

void Drawable::SetTexture(const Texture& texture) {
  texture_ = &texture;
}

const glm::vec4& Drawable::color() const {
  return color_;
}

const Texture* Drawable::texture() const {
  return texture_;
}

} // namespace smk