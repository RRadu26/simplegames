#pragma once
#include "components/simple_scene.h"
#include "lab_m1/lab3/object2D.h"

namespace m1
{
    class UI
    {
     public:
        UI(float x, float y);
        ~UI();
        Mesh *hearth;
        Mesh *bullet;
        Mesh *score;
        Mesh *buttonr;
        Mesh *buttong;

        glm::mat3 bulletpoz(int no);
        glm::mat3 hearthpoz(int no);
        glm::mat3 scorepoz(int no);

     private:
        float lenhearth = 50;
        float lenbullet = 30;
        float lenscore = 25;
        float buttonlen = 300;
        void generateHearts();
        void generateBullets();
        void generateScore();
        void generateButtons(bool end);

        float rezX, rezY;
     protected:

    };
}   // namespace m1