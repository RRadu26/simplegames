#include "lab_m1/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}


void Lab5::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);

}


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab5::Update(float deltaTimeSeconds)
{
    if(projection == 0)
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
    else
        projectionMatrix = glm::ortho(oL, oR, oB, oT, 0.0f, 200.0f);
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }


    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(5, 7, 3.5f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(5, 3, -5));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(0, 1, 1));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(78), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }
    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}


void Lab5::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Lab5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraspeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->TranslateForward(cameraspeed*deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraspeed*deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->TranslateForward(-cameraspeed*deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraspeed*deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-cameraspeed*deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(cameraspeed*deltaTime);

        }
    }

    if (window->KeyHold(GLFW_KEY_V)) {
        fov -= 10 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_B)) {
        fov += 10 * deltaTime;
    }
    
    if (window->KeyHold(GLFW_KEY_N)) {
        oL -= 5 * deltaTime;
        oR += 5 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_M)) {
        oT += 2.5 * deltaTime;
        oB -= 2.5 * deltaTime;
    }
}


void Lab5::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    if (key == GLFW_KEY_P) {
        projection = 0;
    }

    if (key == GLFW_KEY_O) {
        projection = 1;
    }

}


void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0 && renderCameraTarget == false) {

            camera->RotateFirstPerson_OX(-deltaY*sensivityOY);
            camera->RotateFirstPerson_OY(-deltaX*sensivityOX);

        }

        if (window->GetSpecialKeyState() == 0 && renderCameraTarget == true) {

            camera->RotateThirdPerson_OX(-deltaY*sensivityOY);
            camera->RotateThirdPerson_OY(-deltaX*sensivityOX);
        }
    }
}


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab5::OnWindowResize(int width, int height)
{
}
