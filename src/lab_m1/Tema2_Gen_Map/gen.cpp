#include "lab_m1/Tema2_Gen_Map/gen.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
using namespace m1;

Gen::Gen() {
}

Gen::~Gen() {
}
void Gen::loadMap() {
    ifstream is("maps/inferno3.dat", ios::binary);

    vector <glm::vec3> initialPoints;
    int sizeT;
    is.read((char*)&sizeT, 4);
    initialPoints.resize(sizeT);
    is.read((char*)&initialPoints[0], sizeT * sizeof(glm::vec3));
    initialp = initialPoints;
    tra = new trackg(initialPoints);
    CreateMesh("track", tra->vertices, tra->indices);

    vector <glm::vec3> pozbrad;
    is.read((char*)&no_brad, 4);
    pozbrad.resize(no_brad);
    is.read((char*)&pozbrad[0], no_brad * sizeof(glm::vec3));
    for(int i = 0; i < no_brad; i++) {
        glm::mat4 matbrad = glm::mat4(1);
        matbrad = glm::translate(matbrad, pozbrad[i]);
        poz_brad.push_back(matbrad);
    }

    is.read((char*)&poz_start, sizeof(glm::mat4));

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
void Gen::loadGraphics() {
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2_Gen_Map", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2_Gen_Map", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
        createTerrain();
    }
    {
        Mesh* mesh = new Mesh("mertan");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "Tema2"), "mertan.obj");
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
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
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

    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2_Gen_Map", "textures");
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
}

void Gen::Init() {
    loadMap();
    loadGraphics();

    poz_sphere = glm::mat4(1);
    poz_sphere = glm::scale(poz_sphere, glm::vec3(0.25));
    carMatrix = glm::mat4(1);

    camera = new implemented::Camera_gen();
    camera->Set(glm::vec3(39, 0.7, -13.3), glm::vec3(39, 0.4, -14.3), glm::vec3(0, 1, 0));
    carMatrix = glm::translate(carMatrix, camera->GetTargetPosition());
    camera->RotateThirdPerson_OY(RADIANS(45));
    carMatrix = glm::rotate(carMatrix, RADIANS(45), glm::vec3(0, 1, 0));
    carMatrix[3].y = 0;

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

Mesh* Gen::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices) {
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

void Gen::createTerrain() {
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

void Gen::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5, 0.5, 1, 0.8);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Gen::Update(float deltaTimeSeconds) {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], poz_sphere, mapTextures["cube"]);

    RenderSimpleMesh(meshes["mertan"], shaders["LabShader"], carMatrix, mapTextures["cube"]);
    glm::mat4 mm = glm::mat4(1);
    RenderSimpleMesh(meshes["grass"], shaders["LabShader"], mm, mapTextures["Grass"]);
    
    RenderSimpleMesh(meshes["track"], shaders["LabShader"], mm, mapTextures["Road"]);

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

void Gen::FrameEnd() {
    //DrawCoordinateSystem();
}


void Gen::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1)
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

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
 
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
 
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Gen::OnInputUpdate(float deltaTime, int mods) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
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
        return;
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        poz_sphere = glm::translate(poz_sphere, glm::vec3(0, 0, 20*deltaTime));
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        poz_sphere = glm::translate(poz_sphere, glm::vec3(20*deltaTime, 0, 0));

    }
    if (window->KeyHold(GLFW_KEY_S)) {
        poz_sphere = glm::translate(poz_sphere, glm::vec3(0, 0, -20*deltaTime));
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        poz_sphere = glm::translate(poz_sphere, glm::vec3(-20*deltaTime, 0, 0));
    }

}
void Gen::OnKeyPress(int key, int mods) {
    if (window->KeyHold(GLFW_KEY_1)) {
        object = 1;
    }
    if (window->KeyHold(GLFW_KEY_2)) {
        object = 2;
    }
    if (window->KeyHold(GLFW_KEY_3)) {
        object = 3;
    }
    if (window->KeyHold(GLFW_KEY_4)) {
        object = 4;
    }
    if (window->KeyHold(GLFW_KEY_5)) {
        object = 5;
    }
    if (window->KeyHold(GLFW_KEY_ENTER)) {
        insert();
    }
    if (window->KeyHold(GLFW_KEY_BACKSPACE)) {
        del();
    }
    if (window->KeyHold(GLFW_KEY_P)) {
        save();
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        rotation -= 0.1;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        rotation += 0.1;
    }
}


void Gen::OnKeyRelease(int key, int mods) {
}

void Gen::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        camera->RotateFirstPerson_OX(-deltaY*sensivityOY);
        camera->RotateFirstPerson_OY(-deltaX*sensivityOX);
    }
}

void Gen::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
}

void Gen::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
}

void Gen::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void Gen::OnWindowResize(int width, int height) {
}
void Gen::save() {
    vector<glm::vec3> initialPoints = initialp;
    int size = initialPoints.size();
    ofstream fout("maps/inferno3.dat", ios::out | ios::binary);
    fout.write((const char*)&size, 4);
    fout.write((char*)&initialPoints[0], initialPoints.size() * sizeof(glm::vec3));

    vector<glm::vec3> treepoz;
    for(int i = 0; i < no_brad; i++) {
        treepoz.push_back(glm::vec3(poz_brad[i][3]));
    }
    fout.write((const char*)&no_brad, 4);
    fout.write((char*)&treepoz[0], treepoz.size() * sizeof(glm::vec3));

    fout.write((const char*)&poz_start, sizeof(glm::mat4));

    fout.write((const char*)&no_bush, 4);
    fout.write((char*)&poz_bush[0], poz_bush.size() * sizeof(glm::mat4));

    fout.write((const char*)&no_curba_dreapta, 4);
    fout.write((char*)&poz_curba_dreapta[0], poz_curba_dreapta.size() * sizeof(glm::mat4));

    fout.write((const char*)&no_curba_stanga, 4);
    fout.write((char*)&poz_curba_stanga[0], poz_curba_stanga.size() * sizeof(glm::mat4));

    fout.close();
}
void Gen::insert() {
    glm::mat4 objMat = glm::mat4(1);
    glm::vec3 poz = glm::vec3(poz_sphere[3]);
    poz.y = -0.02;
    objMat = glm::translate(objMat, glm::vec3(poz));
    objMat = glm::rotate(objMat, rotation, glm::vec3(0, 1, 0));

    if(object == 1) {
        no_brad++;
        poz_brad.push_back(objMat);
    }
    if(object == 2) {
        poz_start = objMat;
    }
    if(object == 3) {
        no_bush++;
        poz_bush.push_back(objMat);
    }
    if(object == 4) {
        no_curba_dreapta++;
        poz_curba_dreapta.push_back(objMat);
    }
    if(object == 5) {
        no_curba_stanga++;
        poz_curba_stanga.push_back(objMat);
    }
}
void Gen::del() {
    if(object == 1) {
        no_brad--;
        poz_brad.pop_back();
    }
    if(object == 3) {
        no_bush--;
        poz_bush.pop_back();
    }
    if(object == 4) {
        no_curba_dreapta--;
        poz_curba_dreapta.pop_back();
    }
    if(object == 5) {
        no_curba_stanga--;
        poz_curba_stanga.pop_back();
    }
}