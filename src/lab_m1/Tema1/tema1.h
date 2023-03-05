#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/duck.h"
#include "lab_m1/Tema1/UI.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RunGame(float deltaTimeSeconds);
        void mainMenu();
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:

      float translateX, translateY;
      float scaleX, scaleY;
      float angularStep;
      duck *twitty;
      UI *pencil;
      bool rendercol = false;
      glm::mat3 scorepoz;

      int lives = 3;
      int bullets = 3;
      int score = 24;
      bool menu = true;
      bool exit = false;
    };
}   // namespace m1
