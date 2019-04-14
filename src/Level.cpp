#include "Level.hpp"
#include <algorithm>
#include <smk/Input.hpp>
#include <smk/Shape.hpp>
#include <smk/Text.hpp>
#include <smk/View.hpp>
#include "Lang.hpp"

float InRange(float x, float a, float b) {
  if (x < a)
    return a;
  else if (x > b)
    return b;
  else
    return x;
}

inline bool Space(char c) {
  return isspace(c);
}

inline bool NotSpace(char c) {
  return !isspace(c);
}

std::vector<std::string> SplitString(const std::string text) {
  typedef std::string::const_iterator iter;
  std::vector<std::string> ret;
  iter i = text.begin();
  while (i != text.end()) {
    i = std::find_if(i, text.end(), NotSpace);
    iter j = std::find_if(i, text.end(), Space);
    if (i != text.end()) {
      ret.push_back(std::string(i, j));
      i = j;
    }
  }
  return ret;
}

template <class T>
bool FromString(T& t,
                const std::string& s,
                std::ios_base& (*f)(std::ios_base&)) {
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

Level::Level() {
  heroSelected = 0;
  nbHero = 0;
  fluidViewEnable = true;
  time = 0;
  timeDead = 0;
}

void Level::LoadFromFile(std::string fileName) {
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Impossible to open the file: " << fileName << std::endl;
    return;
  }

  std::string ligne;
  while (getline(file, ligne)) {
    std::vector<std::string> split = SplitString(ligne);
    // blocks
    if (split[0] == "b") {
      int x, y, width, height;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      block_list.push_back(Block(x, y, width, height));
    }
    // Hero
    else if (split[0] == "h") {
      float x, y;
      FromString<float>(x, split[1], std::dec);
      FromString<float>(y, split[2], std::dec);
      hero_list.push_back(Hero(x, y));
      nbHero++;
    }
    // invisible Block
    else if (split[0] == "i") {
      int x, y, width, height;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      invBlock_list.push_back(InvisibleBlock(x, y, width, height));
    }
    // FallingBlock
    else if (split[0] == "f") {
      int x, y;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      fallBlock_list.push_back(FallingBlock(x, y));
    }
    // MovableBlock
    else if (split[0] == "m") {
      int x, y;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      movableBlock_list.push_back(MovableBlock(x, y));
    }
    // MovingBlock
    else if (split[0] == "mm") {
      int x, y, width, height;
      float xspeed, yspeed;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      FromString<float>(xspeed, split[5], std::dec);
      FromString<float>(yspeed, split[6], std::dec);
      movBlock_list.push_back(MovingBlock(x, y, width, height, xspeed, yspeed));
    }
    // FinishBlock
    else if (split[0] == "e") {
      int x, y, width, height;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      enddingBlock = FinishBlock(x, y, width, height);
    }
    // setting the view
    else if (split[0] == "v") {
      int xmin, xmax, ymin, ymax;
      FromString<int>(xmin, split[1], std::dec);
      FromString<int>(ymin, split[2], std::dec);
      FromString<int>(xmax, split[3], std::dec);
      FromString<int>(ymax, split[4], std::dec);
      viewXMin = xmin;
      viewYMin = ymin;
      viewXMax = xmax;
      viewYMax = ymax;
    }
    // adding LaserTurret
    else if (split[0] == "l") {
      int x, y, angle, xattach, yattach, mode, angleSpeed;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(angle, split[3], std::dec);
      FromString<int>(xattach, split[4], std::dec);
      FromString<int>(yattach, split[5], std::dec);
      FromString<int>(mode, split[6], std::dec);
      FromString<int>(angleSpeed, split[7], std::dec);
      laserTurret_list.push_back(
          LaserTurret(x, y, angle, xattach, yattach, mode, angleSpeed));
    }
    // adding Glass
    else if (split[0] == "g") {
      int x, y;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      glassBlock_list.push_back(Glass(x, y));
    }
    // adding noDrawBlock
    else if (split[0] == "nd") {
      int x, y, width, height;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      block_list.push_back(Block(x, y, width, height, false));
    }
    // adding Teleporter
    else if (split[0] == "t") {
      int x, y, width, height, xTeleport, yTeleport;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      FromString<int>(xTeleport, split[5], std::dec);
      FromString<int>(yTeleport, split[6], std::dec);
      teleporter_list.push_back(
          Teleporter(x, y, width, height, xTeleport, yTeleport));
      fluidViewEnable = false;
    }
    // adding Electricity
    else if (split[0] == "elec") {
      int x1, y1, x2, y2;
      float ratio;
      int periode;
      int offset;
      FromString<int>(x1, split[1], std::dec);
      FromString<int>(y1, split[2], std::dec);
      FromString<int>(x2, split[3], std::dec);
      FromString<int>(y2, split[4], std::dec);
      FromString<float>(ratio, split[5], std::dec);
      FromString<int>(periode, split[6], std::dec);
      FromString<int>(offset, split[7], std::dec);
      electricity_list.push_back(
          Electricity(x1, y1, x2, y2, ratio, periode, offset));
    }
    // adding cloneurs
    else if (split[0] == "clone") {
      int x1, y1, x2, y2;

      FromString<int>(x1, split[1], std::dec);
      FromString<int>(y1, split[2], std::dec);
      FromString<int>(x2, split[3], std::dec);
      FromString<int>(y2, split[4], std::dec);
      cloneur_list.push_back(Cloner(x1, y1, x2, y2));
    }
    // adding Decor back
    else if (split[0] == "d0") {
      int x, y, img;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(img, split[3], std::dec);
      decorBack_list.push_back(Decor(x, y, img));
    }
    // adding Decor front
    else if (split[0] == "d1") {
      int x, y, img;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(img, split[3], std::dec);
      decorFront_list.push_back(Decor(x, y, img));
    }
    // adding static miroir
    else if (split[0] == "staticMirror") {
      int x1, y1, x2, y2, xattach, yattach;
      FromString<int>(x1, split[1], std::dec);
      FromString<int>(y1, split[2], std::dec);
      FromString<int>(x2, split[3], std::dec);
      FromString<int>(y2, split[4], std::dec);
      FromString<int>(xattach, split[5], std::dec);
      FromString<int>(yattach, split[6], std::dec);
      staticMiroir_list.push_back(
          StaticMirror(x1, y1, x2, y2, xattach, yattach));
    }
    // adding creepers
    else if (split[0] == "creeper") {
      int x, y;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      creeper_list.push_back(Creeper(x, y));
    }
    // adding Arrow launcher
    else if (split[0] == "arrowLauncher") {
      float x, y, orientation;
      FromString<float>(x, split[1], std::dec);
      FromString<float>(y, split[2], std::dec);
      FromString<float>(orientation, split[3], std::dec);
      arrowLauncher_list.push_back(ArrowLauncher(x, y, orientation));
      block_list.push_back(Block(x, y, 32, 32, false));
    }
    // adding ArrowLauncherDetector
    else if (split[0] == "arrowLauncherDetector") {
      int x, y, width, height, mode, ID;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      FromString<int>(mode, split[5], std::dec);
      FromString<int>(ID, split[6], std::dec);
      arrowLauncherDetector_list.push_back(
          ArrowLauncherDetector(x, x + width, y, y + height, mode, ID));
    }
    // adding Pincette
    else if (split[0] == "pincette") {
      pincette_list.push_back(Pincette());
    }
    // adding TextPopup
    else if (split[0] == "textpopup") {
      int m;
      FromString<int>(m, split[1], std::dec);
      textpopup_list.push_back(TextPopup(m));
    }
    // adding Special
    else if (split[0] == "special") {
      int m;
      FromString<int>(m, split[1], std::dec);
      special_list.push_back(Special(m));
    }
    // adding Detector
    else if (split[0] == "detector") {
      int x, y, width, height;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      detector_list.push_back(Detector(x, y, x + width, y + height));
    }
    // adding Pic
    else if (split[0] == "pic") {
      int x, y, angle, nbRequis, comparateur;
      std::vector<int> connexion;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(angle, split[3], std::dec);
      FromString<int>(nbRequis, split[4], std::dec);
      FromString<int>(comparateur, split[5], std::dec);
      unsigned int size = split.size();
      for (unsigned int i = 6; i < size; i++) {
        int detectorId;
        FromString<int>(detectorId, split[i], std::dec);
        connexion.push_back(detectorId);
      }
      pic_list.push_back(Pic(x, y, angle, nbRequis, comparateur, connexion));
    }
    // adding Accelerator
    else if (split[0] == "accelerator") {
      int x, y, width, height, xacc, yacc;
      float viscosite;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(width, split[3], std::dec);
      FromString<int>(height, split[4], std::dec);
      FromString<int>(xacc, split[5], std::dec);
      FromString<int>(yacc, split[6], std::dec);
      FromString<float>(viscosite, split[7], std::dec);
      accelerator_list.push_back(
          Accelerator(x, y, x + width, y + height, xacc, yacc, viscosite));
    }
    // adding Button
    else if (split[0] == "button") {
      int x, y, n;
      FromString<int>(x, split[1], std::dec);
      FromString<int>(y, split[2], std::dec);
      FromString<int>(n, split[3], std::dec);
      button_list.push_back(Button(x, y, n));
    }
  }
  file.close();

  // definition du background;
  spriteBackground.SetTexture(img_background);

  // end of Level variables
  isWin = false;
  isEscape = false;
  isLose = false;

  // The initial view position.
  auto geometry = hero_list[heroSelected].geometry;
  {
    xcenter = geometry.left;
    ycenter = geometry.left;
    SetView();
  }
}

void Level::Draw(smk::Screen& screen) {
  screen.SetView(view_);

  // clang-format off
  for (int x = xcenter - 320 - int(xcenter / 2.67) % 24; x < xcenter + 320; x += 24) {
  for (int y = ycenter - 240 - int(ycenter / 2.67) % 24; y < ycenter + 240; y += 24) {
      spriteBackground.SetPosition(x, y);
      screen.Draw(spriteBackground);
    }
  }

  for (auto& it : special_list) it.DrawBackground(screen, xcenter, ycenter);
  for (auto& it : decorBack_list) it.Draw(screen);
  for (auto& it : special_list) it.DrawOverDecoration(screen);
  // clang-format on


  // Draw static turrets and throw out Laser
  for (auto& it : laserTurret_list) {
    it.Draw(screen);
    Laser(screen, it.x, it.y, it.angle, 10);
  }

  // clang-format off
  for (auto& it : block_list) it.Draw(screen);
  for (auto& it : invBlock_list) it.Draw(screen, hero_list[heroSelected]);
  for (auto& it : movBlock_list) it.Draw(screen);
  for (auto& it : fallBlock_list) it.Draw(screen);
  for (auto& it : movableBlock_list) it.Draw(screen);
  for (auto& it : glassBlock_list) it.Draw(screen);
  for (auto& it : staticMiroir_list) it.Draw(screen);
  for (auto& it : pic_list) it.Draw(screen);
  for (auto& it : special_list) it.DrawForeground(screen, isWin);
  for (auto& it : button_list) it.Draw(screen);
  int i = 0;
  for (auto& it : hero_list) it.Draw(screen, heroSelected == i++);
  for (auto& it : creeper_list) it.Draw(screen);
  for (auto& it : arrow_list) it.Draw(screen);
  for (auto& it : arrowLauncher_list) it.Draw(screen);
  for (auto& it : cloneur_list) it.Draw(screen);
  for (auto& it : particule_list) it.Draw(screen);
  // clang-format on


  // Draw Electricity
  for (auto& it : electricity_list) {
    int x1 = (it.x1);
    int x2 = (it.x2);
    int y1 = (it.y1);
    int y2 = (it.y2);
    smk::Sprite sprite;
    sprite.SetTexture(img_electricitySupport);
    sprite.SetPosition(x1 - 8, y1 - 8);
    screen.Draw(sprite);
    sprite.SetPosition(x2 - 8, y2 - 8);
    screen.Draw(sprite);

    /*
    if (time==100)
    {
            cout<<time<<std::endl;
            cout<<it.offset<<std::endl;
            cout<<it.periode<<std::endl;
            cout<<it.ratio<<std::endl;
            cout<<std::endl;
    }*/

    if (((time + it.offset) % (it.periode)) < it.ratio * it.periode) {
      if (!it.isSound) {
        it.sound.Play();
        it.isSound = true;
      }
      // collision with an Hero

      int i = 0;
      for (auto& it : hero_list) {
        if (IsCollision(it.geometry.increase(10, 10), Line(x1, y1, x2, y2))) {
          it.life -= 4;
        }
        i++;
      }
      DrawElectricity(screen, x1, y1, x2, y2);
      DrawElectricity(screen, x1, y1, x2, y2);
      DrawElectricity(screen, x1, y1, x2, y2);
    } else {
      if (it.isSound) {
        it.sound.Stop();
        it.isSound = false;
      }
    }
  }

  for (auto& pincette : pincette_list) pincette.Draw(screen);
  for (auto& it : decorFront_list) it.Draw(screen);

  // drawing life bar
  smk::Sprite Coeur;
  Coeur.SetTexture(img_coeur);
  if (!hero_list.empty()) {
    int nbCoeur = hero_list[heroSelected].life;
    for (int i = 1; i <= nbCoeur; i++) {
      Coeur.SetPosition(xcenter + i * 16 - 320, ycenter + 220);
      screen.Draw(Coeur);
    }
  }

  // Draw popup
  for (auto it = drawn_textpopup_list.begin(); it != drawn_textpopup_list.end();
       ++it) {
    it->Draw(screen);
  }
}

void Level::Step(smk::Screen& screen) {
  // set the view
  SetView();

  // flush event;
  // Event ev;
  // while (screen.GetEvent(ev)) {
  // if (ev.Type == sf::Event::Closed)
  // isEscape = true;
  //}

  /////////////////////////////////
  //        extra key            //
  /////////////////////////////////

  // test if the player will end the game
  // if (screen.GetInput().IsKeyDown(Key::Escape)) {
  // isEscape = true;
  //}
  // test if the player will restart the room
  if (smk::Input::IsKeyPressed(GLFW_KEY_R)) {
    isLose = true;
  }
  if (smk::Input::IsKeyPressed(GLFW_KEY_T)) {
    isWin = true;
  }

  // Drawn popup
  for (auto it = drawn_textpopup_list.begin(); it != drawn_textpopup_list.end(); ++it) {
    if (it->Step())
      drawn_textpopup_list.erase(it);
    return;
  }

  //////////////////////////////////
  //           testing tools      //
  //////////////////////////////////

  time++;
  /////////////////////////////////
  //        Hero  selected       //
  /////////////////////////////////

  // changement de joueur
  static bool spacepressed = false;
  if (!hero_list.empty()) {
    if (smk::Input::IsKeyHold(GLFW_KEY_SPACE)) {
      if (spacepressed == false) {
        spacepressed = true;
        heroSelected = (heroSelected + 1) % nbHero;
      }
    } else
      spacepressed = false;
  }

  int i = 0;

  if (hero_list.empty()) {
    timeDead++;
    if (timeDead > 30)
      isLose = true;
  }

  for(auto it = hero_list.begin(); it != hero_list.end(); ++it){
    auto& hero = *it;
    // an Hero is dead?
    if (hero.life <= 0) {
      // throw Particule (ghost))
      particule_list.push_front(particuleDead(hero.x, hero.y));

      // we kill him
      it = hero_list.erase(it);
      nbHero--;

      // selected another alive Hero
      if (heroSelected == i) {
        heroSelected = 0;
      } else {
        if (heroSelected > i)
          heroSelected--;
      }

      break;
    }

    // test if there are a ground under the feets of the Hero
    if (PlaceFree(hero, 0, 2)) {
      // apply gravity
      hero.yspeed += 1.7;

    } else {
      if (hero.yspeed < 0)
        hero.yspeed = 0;

      // test for un jump
      if (i == heroSelected) {
        if (smk::Input::IsKeyHold(GLFW_KEY_W) ||
            smk::Input::IsKeyHold(GLFW_KEY_UP)) {
          hero.yspeed = -20;
        }
      }
    }

    if (i == heroSelected) {
      // move on the right
      if (smk::Input::IsKeyHold(GLFW_KEY_D) ||
          smk::Input::IsKeyHold(GLFW_KEY_RIGHT)) {
        hero.sens = false;
        hero.xspeed += 2;
      }

      // move on the left
      if (smk::Input::IsKeyHold(GLFW_KEY_A) ||
          smk::Input::IsKeyHold(GLFW_KEY_LEFT)) {
        hero.sens = true;
        hero.xspeed -= 2;
      }
    }
    // apply friction
    hero.xspeed *= 0.75;
    hero.yspeed *= 0.95;

    // test if we can apply the speed to the position, if not we reduce it while
    // its too high
    if (hero.xspeed != 0) {
      if (!PlaceFree(hero, hero.xspeed, 0)) {
        float i = hero.xspeed;
        while (!PlaceFree(hero, i, 0))
          i -= Sign(hero.xspeed);
        hero.xspeed = i;
      }
    }

    // test if we can apply the speed to the position, if not we reduce it while
    // its too high
    if (hero.yspeed != 0) {
      if (!PlaceFree(hero, 0, hero.yspeed)) {
        float i = hero.yspeed;
        while (!PlaceFree(hero, 0, i))
          i -= Sign(hero.yspeed);
        hero.yspeed = i;
      }
    }

    hero.y += hero.yspeed;
    hero.x += hero.xspeed;

    hero.UpdateGeometry();
    ++i;
  }

  /////////////////////////////////
  //        movingBlocks         //
  /////////////////////////////////

  for (auto& it : movBlock_list) {
    // test if there are an Hero on it.
    for (std::vector<Hero>::iterator itHero = hero_list.begin();
         itHero != hero_list.end(); ++itHero) {
      if (IsCollision(it.geometry, (*itHero).geometry.shift(0, 2))) {
        if (PlaceFree((*itHero), it.xspeed,
                      it.yspeed))  // can we shift the levelHero
        {
          (*itHero).x += it.xspeed;
          (*itHero).y += it.yspeed;
          (*itHero).UpdateGeometry();
        }
      }
    }

    // test if we can apply speed
    if (!PlaceFree(it, it.xspeed, it.yspeed)) {
      for (std::vector<Hero>::iterator itHero = hero_list.begin();
           itHero != hero_list.end(); ++itHero) {
        if (IsCollision(it.geometry.shift(it.xspeed, it.yspeed),
                        (*itHero).geometry))  // collision with levelHero?
        {
          if (PlaceFree((*itHero), it.xspeed,
                        it.yspeed))  // can we shift the levelHero
          {
            (*itHero).x += it.xspeed;
            (*itHero).y += it.yspeed;
            (*itHero).UpdateGeometry();
          }
        }
      }
      if (PlaceFree(it, it.xspeed,
                    it.yspeed))  // after that, can we move?
      {
        it.x += it.xspeed;
        it.y += it.yspeed;
        it.UpdateGeometry();
      } else {
        it.xspeed *= -1;
        it.yspeed *= -1;
      }
    } else {
      it.x += it.xspeed;
      it.y += it.yspeed;
      it.UpdateGeometry();
    }
  }

  /////////////////////////////////
  //        FallingBlock         //
  /////////////////////////////////

  for (auto& it : fallBlock_list) {
    if (it.etape == 0)  // here the FallingBlock still immobile
    {
      if (!PlaceFree(it, 0, -1.5)) {
        it.etape = 1;
      }
    } else if (it.etape > 12)  // here it falls
    {
      it.etape++;
      // test if there are a ground under the feets of the FallingBlock
      if (PlaceFree(it, 0, 1)) {
        // apply gravity
        it.yspeed += 1.7;
      } else {
        it.yspeed = 0;
      }

      // apply friction
      it.yspeed *= 0.95;

      // test if we can apply the speed to the position, if not we reduce it
      // while its too high
      if (it.yspeed != 0) {
        if (!PlaceFree(it, 0, it.yspeed)) {
          float i = it.yspeed;
          while (!PlaceFree(it, 0, i))
            i -= Sign(it.yspeed);
          it.yspeed = i;
        }
      }

      it.y += it.yspeed;
      it.UpdateGeometry();
    } else  // here it wait and shake
    {
      it.etape++;
    }
  }

  /////////////////////////////////
  //        MovableBlock         //
  /////////////////////////////////
  for (auto& it : movableBlock_list) {
    // test if there are a ground under the feets of the MovableBlock
    if (PlaceFree(it, 0, 2)) {
      // apply gravity
      it.yspeed += 1.7;
    } else {
      it.yspeed = 0;
    }

    for (std::vector<Hero>::iterator itHero = hero_list.begin();
         itHero != hero_list.end(); ++itHero) {
      // move on the right
      if (IsCollision(it.geometry.shift(-1, 0), (*itHero).geometry)) {
        if (PlaceFree(it, +1, 0))
          it.xspeed += 2;
      }

      // move on the left
      if (IsCollision(it.geometry.shift(1, 0), (*itHero).geometry)) {
        if (PlaceFree(it, -1, 0))
          it.xspeed -= 2;
      }
    }

    // apply friction
    it.xspeed *= 0.4;
    it.yspeed *= 0.95;

    // test if we can apply the speed to the position, if not we reduce it while
    // its too high
    if (it.xspeed != 0) {
      if (!PlaceFree(it, it.xspeed, 0)) {
        float i = it.xspeed;
        while (!PlaceFree(it, i, 0)) {
          i -= Sign(it.xspeed);
          if (i * it.xspeed <= 0) {
            i = 0;
            break;
          }
        }
        it.xspeed = i;
      }
    }

    // test if we can apply the speed to the position, if not we reduce it while
    // its too high
    if (it.yspeed != 0) {
      if (!PlaceFree(it, 0, it.yspeed)) {
        float i = it.yspeed;
        while (!PlaceFree(it, 0, i)) {
          i -= Sign(it.yspeed);
          if (i * it.yspeed <= 0) {
            i = 0;
            break;
          }
        }
        it.yspeed = i;
      }
    }

    it.y += it.yspeed;
    it.x += it.xspeed;
    it.UpdateGeometry();
  }

  /////////////////////////////////
  //        Glass                //
  /////////////////////////////////
  for (auto& it : glassBlock_list) {
    // test if there are a ground under the feets of the Glass
    if (PlaceFree(it, 0, 1)) {
      // apply gravity
      it.yspeed += 1.7;
    } else {
      it.yspeed = 0;
    }

    for (std::vector<Hero>::iterator itHero = hero_list.begin();
         itHero != hero_list.end(); ++itHero) {
      // move on the right
      if (IsCollision(it.geometry.shift(-1, 0), (*itHero).geometry)) {
        if (PlaceFree(it, +1, 0))
          it.xspeed += 2;
      }

      // move on the left
      if (IsCollision(it.geometry.shift(1, 0), (*itHero).geometry)) {
        if (PlaceFree(it, -1, 0))
          it.xspeed -= 2;
      }
    }

    // apply friction
    it.xspeed *= 0.4;
    it.yspeed *= 0.95;

    // test if we can apply the speed to the position, if not we reduce it while
    // its too high
    if (it.xspeed != 0) {
      if (!PlaceFree(it, it.xspeed, 0)) {
        float i = it.xspeed;
        while (!PlaceFree(it, i, 0)) {
          i -= Sign(it.xspeed);
          if (i * it.xspeed <= 0) {
            i = 0;
            break;
          }
        }
        it.xspeed = i;
      }
    }

    // test if we can apply the speed to the position, if not we reduce it while
    // its too high
    if (it.yspeed != 0) {
      if (!PlaceFree(it, 0, it.yspeed)) {
        float i = it.yspeed;
        while (!PlaceFree(it, 0, i)) {
          i -= Sign(it.yspeed);
          if (i * it.yspeed <= 0) {
            i = 0;
            break;
          }
        }
        it.yspeed = i;
      }
    }

    it.y += it.yspeed;
    it.x += it.xspeed;
    it.UpdateGeometry();
  }
  /////////////////////////////////
  //        FinishBlock          //
  /////////////////////////////////

  for (std::vector<Hero>::iterator itHero = hero_list.begin();
       itHero != hero_list.end(); ++itHero) {
    if (IsCollision((*itHero).geometry, enddingBlock.geometry)) {
      isWin = true;
    }
  }

  /////////////////////////////////
  //        Detector             //
  /////////////////////////////////
  for (auto& it : detector_list) {
    it.detected = false;
    for (std::vector<Hero>::iterator itHero = hero_list.begin();
         itHero != hero_list.end(); ++itHero) {
      if (IsCollision((*itHero).geometry, it.geometry)) {
        it.detected = true;
      }
    }
  }

  /////////////////////////////////
  //       Pics                  //
  /////////////////////////////////
  for (auto& it : pic_list) {
    int nb = 0;
    for (std::vector<int>::iterator itInt = (it.connexion).begin();
         itInt != (it.connexion).end(); ++itInt) {
      if (detector_list[(*itInt)].detected) {
        nb++;
      }
    }
    bool avancer = false;
    switch (it.comparateur) {
      case 0:
        if (nb < it.nbRequis)
          avancer = true;
        break;
      case 1:
        if (nb == it.nbRequis)
          avancer = true;
        break;
      case 2:
        if (nb > it.nbRequis)
          avancer = true;
        break;
    }

    if (avancer) {
      if (it.avancement < 32)
        it.avancement += 2;
    } else {
      if (it.avancement > 0)
        it.avancement--;
    }

    float rad = float(it.angle) * 0.0174532925;
    float p1x = it.x + (it.avancement + 32) * cos(rad);
    float p1y = it.y - (it.avancement + 32) * sin(rad);
    float p2x = it.x + it.avancement * cos(rad) + 8 * cos(rad + 1.57079633);
    float p2y = it.y - it.avancement * sin(rad) - 8 * sin(rad + 1.57079633);
    float p3x = it.x + it.avancement * cos(rad) - 8 * cos(rad + 1.57079633);
    ;
    float p3y = it.y - it.avancement * sin(rad) + 8 * sin(rad + 1.57079633);
    Line l1(p2x, p2y, p1x, p1y);
    Line l2(p3x, p3y, p1x, p1y);

    for (std::vector<Hero>::iterator itHero = hero_list.begin();
         itHero != hero_list.end(); ++itHero) {
      if (IsCollision(itHero->geometry, l1)) {
        itHero->life = 0;
      } else if (IsCollision(itHero->geometry, l2)) {
        itHero->life = 0;
      }
    }
  }

  /////////////////////////////////
  //        Accelerator         //
  /////////////////////////////////
  for (auto& it : accelerator_list) {
    for (std::vector<Hero>::iterator itHero = hero_list.begin();
         itHero != hero_list.end(); ++itHero) {
      if (IsCollision((*itHero).geometry, it.geometry)) {
        itHero->xspeed += it.xacc;
        itHero->yspeed += it.yacc;
        itHero->xspeed *= it.viscosite;
        itHero->yspeed *= it.viscosite;
      }
    }
  }

  /////////////////////////////////
  //        Creepers             //
  /////////////////////////////////

  for (auto creeper = creeper_list.begin(); creeper != creeper_list.end();) {
    if (!CollisionWithAllBlock(Point(creeper->x + creeper->xspeed * 7, creeper->y))) {
      creeper->x += creeper->xspeed;
    } else {
      creeper->xspeed *= -1;
    }
    creeper->UpdateGeometry();

    if (creeper->mode == 0) {
      creeper->t++;
      for (std::vector<Hero>::iterator itHero = hero_list.begin();
           itHero != hero_list.end(); ++itHero) {
        if (abs((*itHero).x - creeper->x) + abs((*itHero).y - creeper->y) < 100) {
          creeper->mode = 1;
          creeper->t = 0;
        }
      }
      if (CollisionWithAllBlock(creeper->geometry)) {
        creeper->mode = 1;
        creeper->t = 19;
      }
    } else {
      if (creeper->t < 20) {
        creeper->t++;
      } else {
        creeper->mode = 0;
        creeper->t = 0;
        for (int i = 0; i <= 20; i++)
          particule_list.push_front(particuleCreeperExplosion(creeper->x, creeper->y));

        for (std::vector<Hero>::iterator itHero = hero_list.begin();
             itHero != hero_list.end(); ++itHero) {
          float distance2 =
              square((*itHero).x - creeper->x) + square((*itHero).y - creeper->y);
          (*itHero).xspeed +=
              InRange(1500 * ((*itHero).x - creeper->x) / distance2, -20, 20);
          (*itHero).yspeed +=
              InRange(1500 * ((*itHero).y - creeper->y) / distance2, -20, 20);
          (*itHero).life -= 3 * 10000 / int(distance2);
        }

        smk::Sound explosion;
        // explosion.SetBuffer(SB_explosion);
        sound_list.push_front(explosion);
        (*(sound_list.begin())).Play();
        creeper = creeper_list.erase(creeper);
        continue;
      }
    }
    ++creeper;
  }

  //////////////////////
  // Creeper explosion //
  //////////////////////

  for (auto& it : sound_list) {
    // if (it.GetStatus() == Sound::Stopped) {
    // sound_list.erase(it);
    // it = sound_list.begin();
    //}
  }

  /////////////////////////////////
  //        Teleporter           //
  /////////////////////////////////
  for (std::vector<Hero>::iterator itHero = hero_list.begin();
       itHero != hero_list.end(); ++itHero) {
    for (auto& it : teleporter_list) {
      if (IsCollision((*itHero).geometry, it.geometry)) {
        // teleport the Hero
        (*itHero).x += it.xTeleport;
        (*itHero).y += it.yTeleport;
        (*itHero).UpdateGeometry();
      }
    }
  }

  /////////////////////////////////
  //        Cloneurs             //
  /////////////////////////////////

  for (auto& it : cloneur_list) {
    if (it.enable) {
      for (int a = 0; a <= 1; a++) {
        particule_list.push_front(
            particuleCloneur(it.xstart + rand() % 32, it.ystart + 32));
      }
      for (std::vector<Hero>::iterator itHero = hero_list.begin();
           itHero != hero_list.end(); ++itHero) {
        if (IsCollision(Point(it.xstart + 16, it.ystart + 16),
                        (*itHero).geometry.increase(-8, -8))) {
          it.enable = false;
          hero_list.push_back(Hero(it.xend, it.yend));
          nbHero++;
          // emit some particules on the end
          for (int a = 0; a <= 50; a++) {
            particule_list.push_front(
                particuleCloneur(it.xend + rand() % 32, it.yend + 32));
          }

          break;
        }
      }
    }
  }
  ////////////////////////////
  // ArrowLauncherDetector //
  //////////////////////////

  for(auto& arrow_launcher_detector : arrowLauncherDetector_list) {
    if (arrow_launcher_detector.mode != 0) {
      if (arrow_launcher_detector.mode <= 2) {
        bool isDetected = false;
        for (std::vector<Hero>::iterator itHero = hero_list.begin();
             itHero != hero_list.end(); ++itHero) {
          if (IsCollision(arrow_launcher_detector.geometry, itHero->geometry)) {
            isDetected = true;
          }
        }
        if (isDetected) {
          if (arrow_launcher_detector.mode == 1)
            arrow_launcher_detector.mode = 0;
          else if (arrow_launcher_detector.mode == 2)
            arrow_launcher_detector.mode = 3;
          int i = 0;
          for (auto& arrow_launcher: arrowLauncher_list) {
            if (i == arrow_launcher_detector.launcherID) {
              arrow_list.push_front(
                  Arrow(arrow_launcher.x + 16, arrow_launcher.y + 16,
                        17 * cos(arrow_launcher.orientation * .0174532925),
                        -17 * sin(arrow_launcher.orientation * .0174532925)));
              arrow_launcher.sound.Play();
            }
            i++;
          }
        }
      } else if (arrow_launcher_detector.mode == 3) {
        if (arrow_launcher_detector.t++ > 30) {
          arrow_launcher_detector.t = 0;
          arrow_launcher_detector.mode = 2;
        }
      }
    }
  }

  ////////////
  // Arrow///
  //////////
  for (auto& it : arrow_list) {
    it.Step();

    if (!it.damage) {
      // burst Particule
      if (abs(it.xspeed) > 1 or abs(it.yspeed) > 1)
        particule_list.push_front(particuleArrow(it.x, it.y));

      if (CollisionWithAllBlock(it.geometry)) {
        if (abs(it.xspeed) > 1 or abs(it.yspeed) > 1) {
          for (std::vector<Hero>::iterator itHero = hero_list.begin();
               itHero != hero_list.end(); ++itHero) {
            if (IsCollision(it.geometry, (*itHero).geometry)) {
              itHero->life -= 100;
              it.yspeed = 0.01;
            }
          }
        }
        it.xspeed = -0.1 * it.xspeed;
        it.yspeed = -0.1 * it.yspeed;
        while (CollisionWithAllBlock(it.geometry)) {
          it.Step();
        }
        it.xspeed *= -1;
        it.yspeed *= -1;
        it.Step();
        it.xspeed = 0;
        it.yspeed = 0;
        it.damage = true;
      }
    } else {
      if (!CollisionWithAllBlock(it.geometry)) {
        it.yspeed += 1.0;
        if (it.alpha > 10)
          it.alpha -= 10;
        else
          it.alpha = 0;
        it.sprite.SetColor(glm::vec4(255, 255, 255, it.alpha) / 255.f);
      }
    }
  }
  /////////////////////////////////
  //        particules           //
  /////////////////////////////////

  for (auto particule = particule_list.begin();
       particule != particule_list.end();) {
    if (particule->Step())
      particule = particule_list.erase(particule);
    else
      ++particule;
  }

  // Pincette
  for (auto& it : pincette_list) it.Step();
  for (auto& special : special_list)
    special.Step(*this);

  ///////////////
  // Button   //
  /////////////

  for (auto& it : button_list) {
    bool pressed = false;
    for (std::vector<Hero>::iterator ithero = hero_list.begin();
         ithero != hero_list.end(); ++ithero) {
      if (IsCollision(it.geometry, ithero->geometry)) {
        pressed = true;
      }
    }
    if (pressed) {
      if (not it.isPressed) {
        it.isPressed = true;
        if (it.nb_pressed < it.nb_pressed_required - 1) {
          it.nb_pressed++;
        }
      }
    } else {
      if (it.isPressed) {
        it.isPressed = false;
      }
    }
  }

  for (auto& pincette : pincette_list)
    pincette.Step();

  // Draw static turrets and throw out Laser
  for (auto& it : laserTurret_list) {
    it.Step();
  }

  // TextPopup.
  for (auto it = textpopup_list.begin(); it != textpopup_list.end();) {
    for (auto& hero : hero_list) {
      if (IsCollision(it->geometry, hero.geometry)) {
        drawn_textpopup_list.push_back(*it);
        it = textpopup_list.erase(it);
      } else {
        ++it;
      }
    }
  }
}

bool Level::CollisionWithAllBlock(Rectangle geom) {
  for (auto& it : block_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(geom == it.geometry))
        return true;
    }
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(geom == it.geometry))
        return true;
    }
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(geom == it.geometry))
        return true;
    }
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(geom == it.geometry))
        return true;
    }
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(geom == it.geometry))
        return true;
    }
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(geom == it.geometry))
        return true;
    }
  }
  return false;
}

bool Level::CollisionWithAllBlock(Point p) {
  for (auto& it : hero_list) {
    if (IsCollision(p, it.geometry))
      return true;
  }
  for (auto& it : block_list) {
    if (IsCollision(p, it.geometry))
      return true;
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(p, it.geometry))
      return true;
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(p, it.geometry))
      return true;
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(p, it.geometry))
      return true;
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(p, it.geometry))
      return true;
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(p, it.geometry))
      return true;
  }
  return false;
}

bool Level::PlaceFree(Hero h, float x, float y) {
  Rectangle shifted = h.geometry.shift(x, y);
  for (auto& it : hero_list) {
    if (IsCollision(shifted, it.geometry)) {
      if (!(h.geometry == it.geometry))
        return false;
    }
  }
  for (auto& it : block_list) {
    if (IsCollision(shifted, it.geometry))
      return false;
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(shifted, it.geometry))
      return false;
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(shifted, it.geometry))
      return false;
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(shifted, it.geometry))
      return false;
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(shifted, it.geometry))
      return false;
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(shifted, it.geometry))
      return false;
  }
  return true;
}

bool Level::PlaceFree(MovingBlock m, float x, float y) {
  Rectangle geom = m.geometry.shift(x, y);
  for (auto& it : block_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(m.geometry == it.geometry))
        return false;
    }
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : hero_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  return true;
}

bool Level::PlaceFree(FallingBlock m, float x, float y) {
  Rectangle geom = m.geometry.shift(x, y);
  for (auto& it : block_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(m.geometry == it.geometry))
        return false;
    }
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : hero_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  return true;
}

bool Level::PlaceFree(MovableBlock m, float x, float y) {
  Rectangle geom = m.geometry.shift(x, y);
  for (auto& it : block_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(m.geometry == it.geometry))
        return false;
    }
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : hero_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  return true;
}

bool Level::PlaceFree(Glass m, float x, float y) {
  Rectangle geom = m.geometry.shift(x, y);
  for (auto& it : block_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(geom, it.geometry)) {
      if (!(m.geometry == it.geometry))
        return false;
    }
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  for (auto& it : hero_list) {
    if (IsCollision(geom, it.geometry))
      return false;
  }
  return true;
}

bool Level::CollisionWithAllBlock(Line l) {
  for (auto& it : block_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  for (auto& it : invBlock_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  for (auto& it : movBlock_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  for (auto& it : fallBlock_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  for (auto& it : movableBlock_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  for (auto& it : glassBlock_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  for (auto& it : hero_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  for (auto& it : staticMiroir_list) {
    if (IsCollision(l, it.geometry))
      return true;
  }
  return false;
}

void Level::Laser(smk::Screen& screen,
                  int x,
                  int y,
                  int angle,
                  int recursiveMaxLevel) {
  if (recursiveMaxLevel <= 0)
    return;
  float a = angle * 0.0174532925;
  int max = 1000;
  static int i = 0;
  float l = max;
  int xx = x;
  int yy = y;
  int xxx;
  int yyy;

  // Draw the Laser
  while (l > 0.5) {
    xxx = xx + l * cos(a);
    yyy = yy - l * sin(a);
    if (!CollisionWithAllBlock(Line(xx, yy, xxx, yyy))) {
      for (int r = 1; r <= 4; r += 1) {
        smk::Shape Line = smk::Shape::Line(xx, yy, xxx, yyy, r,
                                           glm::vec4(50, 0, 0, 0) / 255.f);
        Line.SetBlendMode(Blend::Add);
        screen.Draw(Line);
      }
      xx = xxx;
      yy = yyy;
    }
    l /= 2;
  }

  // we move on more Step
  xxx = xx + 1 * cos(a);
  yyy = yy - 1 * sin(a);
  /*
  for(int r=1;r<=4;r+=1)
  {
          smk::Shape Line=smk::Shape::Line(xx,yy,xxx,yyy,r,Color(50,0,0));
          Line.SetBlendMode(Blend::Add);
          screen.Draw(Line);
  }
  */
  xx = xxx;
  yy = yyy;

  // Draw an halo on the impact of the Laser
  i = rand();
  for (int r = 1; r <= 12 + i % 5; r += 1) {
    smk::Shape circle =
        smk::Shape::Circle(xx, yy, r, glm::vec4(10, 0, 0, 0) / 255.f);
    circle.SetBlendMode(Blend::Add);
    screen.Draw(circle);
  }

  // checking impact of the Laser with the Hero
  i = 0;
  for (auto& it : hero_list) {
    if (IsCollision(Point(xx, yy), it.geometry.increase(4, 4))) {
      particule_list.push_front(particuleLaserOnHero(xx, yy, x, y));
      particule_list.push_front(particuleLaserOnHero(xx, yy, x, y));
      particule_list.push_front(particuleLaserOnHero(xx, yy, x, y));
      particule_list.push_front(particuleLaserOnHero(xx, yy, x, y));
      it.life--;
    }
    i++;
  }
  // checking impact of the Laser with Glass
  for (auto it = glassBlock_list.begin(); it != glassBlock_list.end(); ++it) {
    auto& glass = *it;
    if (IsCollision(Point(xx, yy), glass.geometry.increase(5, 5))) {
      particule_list.push_front(particuleLaserOnGlass(xx, yy, x, y));
      particule_list.push_front(particuleLaserOnGlass(xx, yy, x, y));
      particule_list.push_front(particuleLaserOnGlass(xx, yy, x, y));
      particule_list.push_front(particuleLaserOnGlass(xx, yy, x, y));

      glass.height -= 0.2;
      // glass.width-=0.2;
      // glass.x+=0.1;
      glass.y += 0.2;
      glass.UpdateGeometry();
      if (glass.height <= 3)
        it = glassBlock_list.erase(it);
    }
  }
  // checking impact of the Laser with StaticMirror
  for (auto& it : staticMiroir_list) {
    if (IsCollision(Rectangle(xx - 5, xx + 5, yy - 5, yy + 5), it.geometry)) {
      Laser(screen, xx, yy, 2 * it.angle - angle,
            recursiveMaxLevel - 1);  // throw reflection
    }
  }
}

void Level::DrawElectricity(smk::Screen& screen,
                            int x1,
                            int y1,
                            int x2,
                            int y2) {
  int x = x1;
  int y = y1;
  int xx, yy;
  int xfinal = x2;
  int yfinal = y2;
  while (abs(x - xfinal) + abs(y - yfinal) > 5) {
    float angle = atan2(yfinal - y, xfinal - x);
    angle += float(rand() % 10 - 20) * 0.2;
    xx = x - 9 * cos(angle);
    yy = y - 9 * sin(angle);
    for (int r = 3; r <= 10; r += 2) {
      smk::Shape line1 = smk::Shape::Line(
          x, y, xx, yy, r, glm::vec4(242, 224, 58, 20 - r) / 255.f, r,
          glm::vec4(232, 214, 48, 10 - r) / 255.f);
      line1.SetBlendMode(Blend::Add);
      screen.Draw(line1);
    }

    smk::Shape Line =
        smk::Shape::Line(x, y, xx, yy, 2, glm::vec4(252, 234, 68, 255) / 255.f);
    x = xx;
    y = yy;

    screen.Draw(Line);
  }
}

void Level::SetView() {
  if (!hero_list.empty()) {
    auto geometry = hero_list[heroSelected].geometry;
    if (fluidViewEnable) {
      xcenter = (xcenter + 0.1 * geometry.left) / 1.1;
      ycenter = (ycenter + 0.1 * geometry.top) / 1.1;
    } else {
      xcenter = geometry.left;
      ycenter = geometry.left;
    }
    xcenter = std::max(viewXMin + 320, xcenter);
    xcenter = std::min(viewXMax - 320, xcenter);
    ycenter = std::max(viewYMin + 240, ycenter);
    ycenter = std::min(viewYMax - 240, ycenter);
    static float previous = 0;
    previous = xcenter;
    view_.SetCenter(xcenter, ycenter);
  }
  view_.SetSize(640, 480);
}