#include "lab_m1/Tema2/tema2.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
using namespace m1;

Tema2::Tema2() {
}

Tema2::~Tema2() {
}
void Tema2::loadMap() {
    ifstream is("maps/inferno3.dat", ios::binary);

    int sizeT;
    is.read((char*)&sizeT, 4);
    initialPoints.resize(sizeT);
    is.read((char*)&initialPoints[0], sizeT * sizeof(glm::vec3));
    vector <glm::vec3> pozbrad;
    int nobrad = 0;
    is.read((char*)&nobrad, 4);
    pozbrad.resize(nobrad);
    is.read((char*)&pozbrad[0], nobrad * sizeof(glm::vec3));

    no_brad = nobrad;
    for(int i = 0; i < no_brad; i++) {
        glm::mat4 matbrad = glm::mat4(1);
        matbrad = glm::translate(matbrad, pozbrad[i]);
        poz_brad.push_back(matbrad);
    }
    tra = new track(initialPoints);
    CreateMesh("track", tra->vertices, tra->indices);

    is.read((char*)&poz_start, sizeof(glm::mat4));
    poz_start[3].y = -0.08;

    is.read((char*)&no_bush, 4);
    vector<glm::mat4> bush_coord;
    bush_coord.resize(no_bush);
    is.read((char*)&bush_coord[0], no_bush * sizeof(glm::mat4));
    poz_bush = bush_coord;

    is.read((char*)&no_curba_dreapta, 4);
    vector<glm::mat4> curba_dreapta_coord;
    curba_dreapta_coord.resize(no_curba_dreapta);
    is.read((char*)&curba_dreapta_coord[0], no_curba_dreapta * sizeof(glm::mat4));
    poz_curba_dreapta = curba_dreapta_coord;

    is.read((char*)&no_curba_stanga, 4);
    vector<glm::mat4> curba_stanga_coord;
    curba_stanga_coord.resize(no_curba_stanga);
    is.read((char*)&curba_stanga_coord[0], no_curba_stanga * sizeof(glm::mat4));
    poz_curba_stanga = curba_stanga_coord;   

    is.close();

}
void Tema2::loadGraphics() {
    {
        camera = new implemented::CameraTema();
        camera->Set(initialPoints[initialPoints.size() - 3] + glm::vec3(0.3, 0.7, 1), initialPoints[initialPoints.size() - 3] + glm::vec3(0.3, 0.4, 0), glm::vec3(0, 1, 0));
        camera->RotateThirdPerson_OY(RADIANS(31));

        prevCamera = *camera;

        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
        createTerrain();

    }
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("mertan");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "car.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("brad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "brad.obj");
        for(int i = 0; i < mesh->vertices.size(); i++)
            mesh->vertices[i].color = glm::vec3(0, 1, 0);
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("start_sign");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "start_sign.obj");
        for(int i = 0; i < mesh->vertices.size(); i++)
            mesh->vertices[i].color = glm::vec3(0, 1, 0);
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("bush");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "bush.obj");
        for(int i = 0; i < mesh->vertices.size(); i++)
            mesh->vertices[i].color = glm::vec3(0, 1, 0);
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sign");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "semn.obj");
        for(int i = 0; i < mesh->vertices.size(); i++)
            mesh->vertices[i].color = glm::vec3(0, 1, 0);
        meshes[mesh->GetMeshID()] = mesh;
    }
    //textures
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "textures");
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Start_Sign.png").c_str(), GL_REPEAT);
        mapTextures["start_sign"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Grass.png").c_str(), GL_REPEAT);
        mapTextures["Grass"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Road.jpg").c_str(), GL_REPEAT);
        mapTextures["Road"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Brad.png").c_str(), GL_REPEAT);
        mapTextures["Brad"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Semn_curba_stanga.png").c_str(), GL_REPEAT);
        mapTextures["curba_stanga"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Semn_curba_dreapta.png").c_str(), GL_REPEAT);
        mapTextures["curba_dreapta"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Bush.png").c_str(), GL_REPEAT);
        mapTextures["bush"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Car1.png").c_str(), GL_REPEAT);
        mapTextures["car1"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Car2.png").c_str(), GL_REPEAT);
        mapTextures["car2"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Car3.png").c_str(), GL_REPEAT);
        mapTextures["car3"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Car4.png").c_str(), GL_REPEAT);
        mapTextures["car4"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Car5.png").c_str(), GL_REPEAT);
        mapTextures["car5"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Car6.png").c_str(), GL_REPEAT);
        mapTextures["car6"] = texture;
    }
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "Car7.png").c_str(), GL_REPEAT);
        mapTextures["car7"] = texture;
    }
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x*0.8 - 10, resolution.y*0.8-10, resolution.x / 5.f, resolution.y / 5.f);
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::Init() {
    loadMap();
    loadGraphics();

    {
    glm::vec3 v1 = initialPoints[1] - initialPoints[0];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[0]);
    ocarp.push_back(0);
    devcars.push_back(0.5);
    }
    {
    glm::vec3 v1 = initialPoints[33] - initialPoints[32];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[33]);
    ocarp.push_back(32);
    devcars.push_back(-0.5);
    }
    {
    glm::vec3 v1 = initialPoints[71] - initialPoints[70];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[70]);
    ocarp.push_back(70);
    devcars.push_back(0.3);
    }
    {
    glm::vec3 v1 = initialPoints[100] - initialPoints[99];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[99]);
    ocarp.push_back(99);
    devcars.push_back(0.5);
    }
    {
    glm::vec3 v1 = initialPoints[1] - initialPoints[0];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[0]);
    ocarp.push_back(0);
    devcars.push_back(0.5);
    }
    {
    glm::vec3 v1 = initialPoints[33] - initialPoints[32];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[33]);
    ocarp.push_back(32);
    devcars.push_back(-0.5);
    }
    {
    glm::vec3 v1 = initialPoints[71] - initialPoints[70];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[70]);
    ocarp.push_back(70);
    devcars.push_back(0.3);
    }
    {
    glm::vec3 v1 = initialPoints[100] - initialPoints[99];
    v1 = glm::normalize(v1);
    float cos = glm::dot(v1, glm::vec3(0, 0, 1));
    rotationscars.push_back((float)glm::atan2(v1.x, cos));
    otherCars.push_back(initialPoints[99]);
    ocarp.push_back(99);
    devcars.push_back(0.5);
    }

    speed = 0;
    acceleration = 0;
    carMatrix = glm::mat4(1);
    carMatrix = glm::translate(carMatrix, camera->GetTargetPosition());
    carMatrix = glm::rotate(carMatrix, RADIANS(31), glm::vec3(0, 1, 0));
    
    carMatrix[3].y = 0;

}
void Tema2::RenderRace() {
    glm::mat4 amatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carMatrix, mapTextures["car2"]);
    RenderSimpleMesh(meshes["grass"], shaders["LabShader"], amatrix, mapTextures["Grass"]);
    RenderSimpleMesh(meshes["track"], shaders["LabShader"], amatrix, mapTextures["Road"]);
    for(int i = 0 ; i < no_brad ; i ++)
       RenderSimpleMesh(meshes["brad"], shaders["LabShader"], poz_brad[i], mapTextures["Brad"]);
    for(int i = 0 ; i < no_curba_stanga ; i ++)
       RenderSimpleMesh(meshes["sign"], shaders["LabShader"], poz_curba_stanga[i], mapTextures["curba_stanga"]);
    for(int i = 0 ; i < no_curba_dreapta ; i ++)
       RenderSimpleMesh(meshes["sign"], shaders["LabShader"], poz_curba_dreapta[i], mapTextures["curba_dreapta"]);
    for(int i = 0 ; i < no_bush ; i ++)
       RenderSimpleMesh(meshes["bush"], shaders["LabShader"], poz_bush[i], mapTextures["bush"]);
    RenderSimpleMesh(meshes["start_sign"], shaders["LabShader"], poz_start, mapTextures["start_sign"]);

}
Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices) {
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema2::createTerrain() {
    std::vector<VertexFormat> verticesf;
    std::vector<unsigned int> indicesf;
    int sizemap = 280;
    for(int i = 0 ; i < sizemap; i++) {
        for(int j = 0 ; j < sizemap; j++) {
            verticesf.push_back(VertexFormat(glm::vec3((i*2-sizemap)/5 - 10, -0.03,  (j*2-sizemap)/5 - 10), glm::vec3(0, 1, 0), glm::vec3(0.05, 0.5, 0.23)));
        }
    }
    for(int i = 0 ; i < sizemap - 1; i += 1) {
        for(int j = 0 ; j < sizemap - 1; j++) {

            indicesf.push_back(i + sizemap*j);
            indicesf.push_back(i + sizemap*j +sizemap);
            indicesf.push_back(i + sizemap*j + sizemap + 1);
            indicesf.push_back(i + sizemap*j);
            indicesf.push_back(i + sizemap*j +sizemap + 1);
            indicesf.push_back(i + 1);
        }
    }
    CreateMesh("grass", verticesf, indicesf);
}

void Tema2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5, 0.5, 1, 0.8);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    *camera = prevCamera;
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    carMatrix = glm::translate(carMatrix, glm::vec3(0, 0, -speed*deltaTimeSeconds));
    camera->MoveForward(speed*deltaTimeSeconds);

    speed += acceleration;
    insideTrack(carMatrix, deltaTimeSeconds);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
    moveCars2(deltaTimeSeconds);
    moveCars(deltaTimeSeconds);
    RenderRace();
    prevCamera = *camera;
    camera->Set(camera->GetTargetPosition() + glm::vec3(0, 8, 0), camera->GetTargetPosition(), glm::vec3(0, 0 ,1));


    projectionMatrix =glm::ortho(oL, oR, oB, oT, 0.0f, 200.0f);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    moveCars2(0);
    moveCars(0);
    
    RenderRace();
}

void Tema2::FrameEnd() {
    //DrawCoordinateSystem();
}


void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, 0, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    location = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(location, 1, 0, glm::value_ptr(viewMatrix));


    location = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(location, 1, 0, glm::value_ptr(projectionMatrix));

    location = glGetUniformLocation(shader->program, "pozCar");
    glm::vec3 pozCar = glm::vec3(carMatrix[3]);
    glUniform3fv(location, 1, glm::value_ptr(pozCar));
    // Draw the object
    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
 
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
 
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (window->KeyHold(GLFW_KEY_D)) {
        camera->RotateThirdPerson_OY(1.7*-deltaTime);
        prevCamera.RotateThirdPerson_OY(1.7*-deltaTime);
        carMatrix = glm::rotate(carMatrix, 1.7f*-deltaTime, glm::vec3(0, 1, 0));
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        camera->RotateThirdPerson_OY(1.7*deltaTime);
        prevCamera.RotateThirdPerson_OY(1.7*deltaTime);

        carMatrix = glm::rotate(carMatrix, 1.7f*deltaTime, glm::vec3(0, 1, 0));
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        if(speed < 10)
            acceleration = deltaTime;
        else
            acceleration = 0;
    } else if(speed > 0) {
        acceleration = -deltaTime/4;
        if(speed + acceleration < 0) {
            acceleration = 0;
            speed = 0;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if(speed > 0)
            acceleration = -deltaTime * 10;
        else {
            acceleration = 0;
            speed = 0;
        }
    }
    if (window->KeyHold(GLFW_KEY_R)) {
        if(speed <= 0){
            glm::mat4 ncm = carMatrix;
            ncm = glm::translate(ncm, glm::vec3(0, 0, -0.4));
            if(insideTrack(carMatrix, deltaTime)) {
            carMatrix = glm::translate(carMatrix, glm::vec3(0, 0, deltaTime));
            camera->MoveForward(-deltaTime);
            prevCamera.MoveForward(-deltaTime);
            }
        }
    }
}

void Tema2::OnKeyPress(int key, int mods) {
    if (window->KeyHold(GLFW_KEY_F)) {
        camera->MoveForward(camera->distanceToTarget);
        prevCamera.MoveForward(prevCamera.distanceToTarget);
    }
}


void Tema2::OnKeyRelease(int key, int mods) {

}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void Tema2::OnWindowResize(int width, int height) {
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x*0.8 - 10, resolution.y*0.8-10, resolution.x / 5.f, resolution.y / 5.f);
}

float Tema2::area(float x1, float y1, float x2, float y2, float x3, float y3)
{
   return abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}
bool Tema2::isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y) {  
   float A = area (x1, y1, x2, y2, x3, y3);
  
   float A1 = area (x, y, x2, y2, x3, y3);
  
   float A2 = area (x1, y1, x, y, x3, y3);
  
   float A3 = area (x1, y1, x2, y2, x, y);
   return (A - A1 - A2 - A3 > -0.1 && A - A1 - A2 - A3 < 0.1);
}
bool Tema2::insideTrack(glm::mat4 matrix, float deltaTime) {
    int no_tr = tra->indices.size()/3;
    if(!inside)
        matrix = glm::translate(matrix, glm::vec3(0, 0, 0.4));
    for(int i = 0; i < no_tr; i++) {
        int index = i * 3;
        float x1 = tra->vertices[tra->indices[index]].position.x;
        float y1 = tra->vertices[tra->indices[index]].position.z;
        float x2 = tra->vertices[tra->indices[index + 1]].position.x;
        float y2 = tra->vertices[tra->indices[index + 1]].position.z;
        float x3 = tra->vertices[tra->indices[index + 2]].position.x;
        float y3 = tra->vertices[tra->indices[index + 2]].position.z;
        if(isInside(x1, y1, x2, y2, x3, y3, matrix[3].x, matrix[3].z)) {
            inside = true;
            return inside;
        }
    }
    if(inside) {
    speed = 0;
    acceleration = 0;
    }
    inside = false;
    return inside;
}
void Tema2::moveCars(float deltaTime) {
    for(int i=0 ; i < otherCars.size()/2 ; i++) {
        if(deltaTime == 0) 
            break;
        int j = ocarp[i] + 1;
        if(j == initialPoints.size())
            j = 0;
        glm::vec3 v1 = initialPoints[j] - initialPoints[ocarp[i]];
        v1 = glm::normalize(v1);
        if(glm::distance(otherCars[i], initialPoints[j]) < 0.3 ) {
            ocarp[i] = j;
            v1 = initialPoints[j + 1] - initialPoints[ocarp[i]];
            v1 = glm::normalize(v1);
            float cos = glm::dot(v1, glm::vec3(0, 0, 1));
            rotationscars[i] = (float)glm::atan2(v1.x, cos);
        }
        otherCars[i] += v1 * deltaTime * 2.0f;
        if(glm::distance(glm::vec3(carMatrix[3]), otherCars[i] + glm::vec3(0, 0, 0.15)) < 0.15) {
            speed = 0;
            acceleration = 0;
        }
    }
    glm::mat4 carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[0] + glm::vec3(devcars[0], 0, 0));
    carm = glm::rotate(carm, rotationscars[0], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car1"]);

    carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[1] + glm::vec3(devcars[1], 0, 0));
    carm = glm::rotate(carm, rotationscars[1], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car3"]);

    carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[2] + glm::vec3(devcars[2], 0, 0));
    carm = glm::rotate(carm, rotationscars[2], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car4"]);

    carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[3] + glm::vec3(devcars[3], 0, 0));
    carm = glm::rotate(carm, rotationscars[3], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car5"]);
    return;
}
void Tema2::moveCars2(float deltaTime) {
    for(int i = otherCars.size()/2 ; i < otherCars.size() ; i++) {
        if(deltaTime == 0) 
            break;
        int j = ocarp[i] - 1;
        if(j == -1)
            j = initialPoints.size() - 1;
        glm::vec3 v1 = initialPoints[j] - initialPoints[ocarp[i]];
        v1 = glm::normalize(v1);
        if(glm::distance(otherCars[i], initialPoints[j]) < 0.3 ) {
            ocarp[i] = j;
            v1 = initialPoints[j - 1] - initialPoints[ocarp[i]];
            v1 = glm::normalize(v1);
            float cos = glm::dot(v1, glm::vec3(0, 0, 1));
            rotationscars[i] = (float)glm::atan2(v1.x, cos);
        }
        otherCars[i] += v1 * deltaTime * 2.0f;
        if(glm::distance(glm::vec3(carMatrix[3]), otherCars[i] + glm::vec3(0, 0, 0.15)) < 0.15) {
            speed = 0;
            acceleration = 0;
        }
    }
    glm::mat4 carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[4] + glm::vec3(devcars[4], 0, 0));
    carm = glm::rotate(carm, rotationscars[4], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car6"]);

    carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[5] + glm::vec3(devcars[5], 0, 0));
    carm = glm::rotate(carm, rotationscars[5], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car7"]);

    carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[6] + glm::vec3(devcars[6], 0, 0));
    carm = glm::rotate(carm, rotationscars[6], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car3"]);

    carm = glm::mat4(1);
    carm = glm::translate(carm, otherCars[7] + glm::vec3(devcars[7], 0, 0));
    carm = glm::rotate(carm, rotationscars[7], glm::vec3(0, 1, 0));
    carm = glm::rotate(carm, RADIANS(180), glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carm, mapTextures["car1"]);
    return;
}