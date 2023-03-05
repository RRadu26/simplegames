#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/lab3/object2D.h"


using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);
    
    twitty = new duck(resolution.x, resolution.y, &lives, &bullets, &score);
    pencil = new UI(resolution.x, resolution.y);
    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 green(0, 0.46, 0);
    glm::vec3 blue(0, 0.65, 1);

    Mesh *grass = object2D::CreateSquare("grass", corner, 150, resolution.x, green, true);
    Mesh *scorefill = object2D::CreateSquare("scorefill", corner, 25, 300 , blue);
    scorepoz = glm::mat3(1);
    scorepoz *= transform2D::Translate(resolution.x - 300 , resolution.y - 100);

    AddMeshToList(scorefill);
    AddMeshToList(grass);
    AddMeshToList(twitty->mesh);
    AddMeshToList(twitty->rWing);
    AddMeshToList(twitty->lWing);
    AddMeshToList(twitty->collider);
    AddMeshToList(pencil->hearth);
    AddMeshToList(pencil->bullet);
    AddMeshToList(pencil->score);
    AddMeshToList(pencil->buttong);
    AddMeshToList(pencil->buttonr);

    twitty->no_duck = 0;

}

void Tema1::RunGame(float deltaTimeSeconds) {
    if(lives <= 0) {
        menu = true;
        return;
    }

    glm::ivec2 resolution = window->GetResolution();
    twitty->fly(deltaTimeSeconds);
    if (twitty->deleted) {
        delete twitty;
        twitty = new duck(resolution.x, resolution.y, &lives, &bullets, &score);
        twitty->deleted = false;
        twitty->no_duck += 1;

    }
    for(int i = 0 ; i < lives ; i++)
        RenderMesh2D(meshes["hearth"], shaders["VertexColor"], pencil->hearthpoz(i));
    for(int i = 0 ; i < bullets ; i++)
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], pencil->bulletpoz(i));
    for(int i = 0 ; i < score ; i++)
    RenderMesh2D(meshes["score"], shaders["VertexColor"], pencil->scorepoz(i));

    RenderMesh2D(meshes["scorefill"], shaders["VertexColor"], scorepoz);
    RenderMesh2D(meshes["grass"], shaders["VertexColor"], glm::mat3(1));
    RenderMesh2D(meshes["duck"], shaders["VertexColor"], twitty->modelMatrix);
    RenderMesh2D(meshes["rwing"], shaders["VertexColor"], twitty->modelMatrixRW);
    RenderMesh2D(meshes["lwing"], shaders["VertexColor"], twitty->modelMatrixLW);
    if(rendercol)
    RenderMesh2D(meshes["collider"], shaders["VertexColor"], twitty->ColliderMatrix);
}

void Tema1:: mainMenu() {
    glm::ivec2 resolution = window->GetResolution();
    glm::mat3 modelmatrix = glm::mat3(1);
    modelmatrix *= transform2D::Translate((float)resolution.x/2 - 150, (float)resolution.y/2 + 100);
    RenderMesh2D(meshes["greenbutton"], shaders["VertexColor"], modelmatrix);
    modelmatrix *= transform2D::Translate(0, -100);
    RenderMesh2D(meshes["redbutton"], shaders["VertexColor"], modelmatrix);
}
void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.87, 1, 0.46, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds) {
    if(menu) {
        mainMenu();
        return;
    }
    RunGame(deltaTimeSeconds);

}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY =  window->GetResolution().y - mouseY;
    if(menu) {
        glm::ivec2 resolution = window->GetResolution();
        glm::mat3 modelMatrixb = glm::mat3(1);
        modelMatrixb *= transform2D::Translate((float)resolution.x/2 - 150, (float)resolution.y/2 + 100);
        if(mouseX > modelMatrixb[2][0] &&  mouseX < modelMatrixb[2][0] + 300 &&
            mouseY > modelMatrixb[2][1] &&  modelMatrixb[2][1] + 50) {
            menu = false;
            lives = 3;
            bullets = 3;
            score = 0;
            delete twitty;
            twitty = new duck(resolution.x, resolution.y, &lives, &bullets, &score);
            twitty->deleted = false;
            twitty->no_duck = 0;
            return;
            }
        if(mouseX > modelMatrixb[2][0] &&  mouseX < modelMatrixb[2][0] + 300 &&
            mouseY > modelMatrixb[2][1] &&  modelMatrixb[2][1] + 50) 
            exit = true;
        return;
    }
    score -=1;
    if(bullets <= 0) {
        twitty->escaped = true;
        }
    if(mouseX > twitty->ColliderMatrix[2][0] &&  mouseX < twitty->ColliderMatrix[2][0] + 2*twitty->len &&
        mouseY > twitty->ColliderMatrix[2][1] &&  mouseY < twitty->ColliderMatrix[2][1] + 2*twitty->len && bullets >=1 && !twitty->escaped) {
             twitty->dead = true;
             if(score < 24)
                score +=2;
    }
    bullets -= 1;


}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
