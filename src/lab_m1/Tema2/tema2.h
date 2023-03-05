#pragma once

#include <vector>
#include "lab_m1/Tema2/track.h"
#include "lab_m1/Tema2/camera_tema.h"
#include <unordered_map>
#include "components/simple_scene.h"


namespace m1 {
    class Tema2 : public gfxc::SimpleScene {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };
        Tema2();
        ~Tema2();

        void Init() override;
        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

     private:
        void createTerrain();
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D* texture1 = NULL);
        void loadMap();
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        float area(float x1, float y1, float x2, float y2, float x3, float y3);
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);
        bool insideTrack(glm::mat4 matrix, float deltaTime);
        void loadGraphics();
        void RenderRace();
        void moveCars(float deltaTime);
        void moveCars2(float deltaTime);
        track *tra;
        implemented::CameraTema *camera;
        implemented::CameraTema prevCamera;

        glm::mat4 projectionMatrix;
        glm::mat4 carMatrix;
        float speed;
        float acceleration = 0;
        bool inside = true;
        std::vector <glm::vec3> initialPoints;
        std::vector<glm::vec3> otherCars;
        std::vector<int> ocarp;
        std::vector<float> rotationscars;
        std::vector<float> devcars;

        std::unordered_map<std::string, Texture2D *> mapTextures;
        float oL = -7.0f;
        float oR = 7.0f;
        float oT = 4.0f;
        float oB = -4.0f;
        int no_brad = 0;
        int no_curba_stanga = 0;
        int no_curba_dreapta = 0;
        int no_bush = 0;
        std::vector<glm::mat4> poz_brad;
        std::vector<glm::mat4> poz_curba_stanga;
        std::vector<glm::mat4> poz_curba_dreapta;
        std::vector<glm::mat4> poz_bush;
        ViewportArea miniViewportArea;

        glm::mat4 poz_start;
    };
}