#pragma once

#include <vector>
#include "lab_m1/Tema2_Gen_Map/trackg.h"
#include "lab_m1/Tema2_Gen_Map/camera_gen.h"

#include "components/simple_scene.h"
#include <unordered_map>

namespace m1 {
    class Gen : public gfxc::SimpleScene {
     public:
        Gen();
        ~Gen();

        void Init() override;
        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

     private:
        void createTerrain();
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture1 = NULL);
        void loadMap();
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        int object = 1;
        float rotation = 0;
        trackg *tra;
        implemented::Camera_gen *camera;
        glm::mat4 projectionMatrix;
        glm::mat4 carMatrix;
        glm::mat4 poz_sphere;
        std::vector<glm::vec3> initialp;
        void loadGraphics();
        float cameraspeed = 10;
        void save();
        void insert();
        void del();
        std::unordered_map<std::string, Texture2D *> mapTextures;

        int no_brad = 0;
        int no_curba_stanga = 0;
        int no_curba_dreapta = 0;
        int no_bush = 0;
        std::vector<glm::mat4> poz_brad;
        std::vector<glm::mat4> poz_curba_stanga;
        std::vector<glm::mat4> poz_curba_dreapta;
        std::vector<glm::mat4> poz_bush;

        glm::mat4 poz_start;
    };
}