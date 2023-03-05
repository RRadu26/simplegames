#pragma once
#include "components/simple_scene.h"
#include "lab_m1/lab3/object2D.h"

namespace m1
{
    class duck
    {
     public:
        duck(float w, float h, int *lives, int *bullets, int *score);
        ~duck();
        Mesh *mesh;
        Mesh *rWing;
        Mesh *lWing;
        Mesh *collider;

        glm::mat3 modelMatrix;
        glm::mat3 modelMatrixLW;
        glm::mat3 modelMatrixRW;
        glm::mat3 ColliderMatrix;

        void fly(float deltatime);
        inline static int no_duck = 20;
        inline static int deleted = false;
        float rotd = 0;
        float len;
        bool dead = false;
        bool escaped = false;
     private:
        void rotateDuck(float coef);
        void createMesh(const std::string &name, glm::vec3 leftBottomCorner, float length, bool fill = false);
        void createWings(const std::string &name1, const std::string &name2, glm::vec3 leftBottomCorner, float length, bool fill = false);
        void move(float deltatime, float speed);
        void escape(float deltatime);
        void die(float deltatime);
        void animate(float deltatime);
     protected:
        float timer = 0;
        float timerx = 0, timery = 0;
        float movx = 1, movy = 1;
        float prevx = 0, prevy = 0;
        int state = 1;
        float time_alive = 0;
        int w, h;
        float rot = 0;
        float coef = 1;
        int *lives;
        int *bullets;
        bool lifed = false;
        int *score;
        float angle;
    };
}   // namespace m1
