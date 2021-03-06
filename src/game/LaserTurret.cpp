#include "game/LaserTurret.hpp"

#include <cmath>
#include "game/Resource.hpp"
#include <smk/Window.hpp>
#include <smk/Shape.hpp>
#include <smk/Color.hpp>

LaserTurret::LaserTurret(int X,
                         int Y,
                         int Angle,
                         int Xattach,
                         int Yattach,
                         int Mode,
                         int AngleSpeed) {
  x = X;
  y = Y;
  angle = Angle;
  xattach = Xattach;
  yattach = Yattach;
  mode = Mode;
  angleSpeed = AngleSpeed;
  angleIncrement = 0;

  sprite = smk::Sprite(img_turret);
  sprite.SetCenter(10, 3);
  sprite.SetRotation(angle);
  sprite.SetPosition(x, y);

  if (mode == 2)
    angleMedium = Angle;
}

void LaserTurret::Draw(smk::Window& window) {
  auto line =
      smk::Shape::Line(glm::vec2(x, y), glm::vec2(xattach, yattach), 1);
  line.SetColor(smk::Color::Black);
  window.Draw(line);
  window.Draw(sprite);
}

void LaserTurret::Step() {
  switch (mode) {
    case 0:
      break;
    case 1:
      angle += angleSpeed;
      sprite.SetRotation(angle);
      break;
    case 2:
      angleIncrement += angleSpeed;
      angle = angleMedium + 45 * std::sin(angleIncrement * 0.0174532925);
      sprite.SetRotation(angle);
      break;
  }
}
