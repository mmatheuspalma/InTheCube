#include "TextPopup.hpp"

#include "Clock.hpp"
#include "Lang.hpp"
#include "Resource.hpp"
#include "smk/Color.hpp"
#include <smk/Screen.hpp>
#include <smk/Shape.hpp>
#include <smk/Input.hpp>

TextPopup::TextPopup(int t) {
  switch (t) {
    case 0:
      text.push_back(std::vector<std::wstring>());
      geometry.left = 320;
      geometry.right = 320 + 16;
      geometry.top = 160;
      geometry.bottom = 160 + 96;
      text[0].push_back(tr(L"arbret11"));
      text[0].push_back(tr(L"arbret12"));
      text[0].push_back(tr(L"arbret13"));
      text[0].push_back(tr(L"arbret14"));
      break;
    case 1:
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      geometry.left = 576;
      geometry.right = 576 + 32;
      geometry.top = 160;
      geometry.bottom = 448;
      text[0].push_back(tr(L"arbret21"));
      text[1].push_back(tr(L"arbret22"));
      break;
    case 2:
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      text[0].push_back(tr(L"arbret31"));
      text[0].push_back(tr(L"arbret32"));
      text[1].push_back(tr(L"arbret41"));
      text[1].push_back(tr(L"arbret42"));
      text[2].push_back(tr(L"arbret51"));
      text[2].push_back(tr(L"arbret52"));
      text[2].push_back(tr(L"arbret53"));
      text[2].push_back(tr(L"arbret54"));
      text[3].push_back(tr(L"arbret61"));
      text[3].push_back(tr(L"arbret62"));
      text[3].push_back(tr(L"arbret63"));
      text[3].push_back(tr(L"arbret64"));
      text[3].push_back(tr(L"arbret65"));
      text[4].push_back(tr(L"arbret71"));
      text[4].push_back(tr(L"arbret72"));
      text[4].push_back(tr(L"arbret73"));
      text[4].push_back(tr(L"arbret74"));
      text[4].push_back(tr(L"arbret75"));
      text[5].push_back(tr(L"arbret81"));
      text[5].push_back(tr(L"arbret82"));
      text[5].push_back(tr(L"arbret83"));
      text[6].push_back(tr(L"arbret91"));
      text[6].push_back(tr(L"arbret92"));
      text[6].push_back(tr(L"arbret93"));
      text[7].push_back(tr(L"arbret101"));
      text[7].push_back(tr(L"arbret102"));
      text[7].push_back(tr(L"arbret103"));
      text[7].push_back(tr(L"arbret104"));
      text[7].push_back(tr(L"arbret105"));
      text[8].push_back(tr(L"arbret111"));
      text[8].push_back(tr(L"arbret112"));
      break;
    case 3: {
      text.push_back(std::vector<std::wstring>());
      text.push_back(std::vector<std::wstring>());
      geometry.left = 1408;
      geometry.right = 1408 + 128;
      geometry.top = 32;
      geometry.bottom = 480;
      text[0].push_back(tr(L"instruction1"));
      text[0].push_back(tr(L"instruction2"));
      text[0].push_back(tr(L"instruction3"));
      text[0].push_back(tr(L"instruction4"));
      text[0].push_back(tr(L"instruction5"));

      text[1].push_back(tr(L"instruction6"));
      text[1].push_back(tr(L"instruction7"));
      text[1].push_back(tr(L"instruction8"));
    } break;
  }
  spaceSprite.SetTexture(img_decorSpace);
}

bool TextPopup::Step() {
  time++;
  horizontal_shift += (100 - horizontal_shift) / 10.0;

  if (time > 10) {
    if (smk::Input::IsMousePressed(GLFW_MOUSE_BUTTON_1) ||
        smk::Input::IsKeyPressed(GLFW_KEY_SPACE) ||
        smk::Input::IsKeyPressed(GLFW_KEY_ENTER)) {
      p++;
      horizontal_shift = 640;
      time = 0;
      if (p >= text.size())
        return true;
    }
  }
  return false;
}

void TextPopup::Draw(smk::Screen& screen) {
  if (p >= text.size())
    return;
  //FloatRect rect = screen.GetView().GetRect();
  // drawing cadre
  float left = screen.GetView().Left();
  float top = screen.GetView().Top();
  int x1 = left + 640 / 5;
  int y1 = top + 480 / 5 + horizontal_shift;

  int x2 = 640 * 4 / 5 + left;
  int y2 = 480 + horizontal_shift + top;

  int r = 25;
  int e = 10;

  glm::vec4 c0 = {0.05, 0.05, 0.05, 1.0};
  glm::vec4 c1 = smk::Color::White;
  auto blackRect1 = smk::Shape::Rectangle(x1, y1 - r, x2, y2 + r, c0);
  auto blackRect2 = smk::Shape::Rectangle(x1 - r, y1, x2 + r, y2, c0);
  auto whiteRect1 = smk::Shape::Rectangle(x1, y1 - r + e, x2, y2 + r - e, c1);
  auto whiteRect2 = smk::Shape::Rectangle(x1 - r + e, y1, x2 + r - e, y2, c1);
  screen.Draw(blackRect1);
  screen.Draw(blackRect2);
  //Shape blackCircle = Shape::Circle(x1, y1, r, c0);
  //Shape whiteCircle = Shape::Circle(x1, y1, r - e, c1);
  //screen.Draw(blackCircle);
  //screen.Draw(whiteCircle);
  //blackCircle.Move(x2 - x1, 0);
  //whiteCircle.Move(x2 - x1, 0);
  //screen.Draw(blackCircle);
  //screen.Draw(whiteCircle);
  //blackCircle.Move(0, y2 - y1);
  //whiteCircle.Move(0, y2 - y1);
  //screen.Draw(blackCircle);
  //screen.Draw(whiteCircle);
  //blackCircle.Move(x1 - x2, 0);
  //whiteCircle.Move(x1 - x2, 0);
  //screen.Draw(blackCircle);
  //screen.Draw(whiteCircle);
  screen.Draw(whiteRect1);
  screen.Draw(whiteRect2);

  // drawing texte
  int x = x1 + 5;
  int y = y1 + 5;
  textString.SetColor(c0);
  textString.SetFont(font_arial);
  for (auto& t : text[p]) {
    textString.SetPosition(x, y);
    textString.SetString(t);
    screen.Draw(textString);
    y += 40;
  }
  spaceSprite.SetPosition(x2 - 128, y2 - 135);
  screen.Draw(spaceSprite);
}