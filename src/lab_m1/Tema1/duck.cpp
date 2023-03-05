#include "lab_m1/Tema1/duck.h"
#include "lab_m1/Tema1/transform2D.h"
#include "iostream"
#include "stdlib.h"
#include <time.h> 
using namespace m1;

duck::duck(float w, float h, int *lives, int *bullets, int *score)
{
    
    glm::vec3 corner = glm::vec3(0, 0, 0);
    len = 70;    
    float lenW = 110;
    createMesh("duck", corner, len, true);
    createWings("rwing", "lwing", corner, lenW, true);
    modelMatrix = glm::mat3(1);
    srand (time(NULL));
    float randomSpawn = random() % (long)((w-100)-100+1) + 100;
    float randomAngle = random() % 2;
    modelMatrix *= transform2D::Translate(randomSpawn, 150);
    ColliderMatrix = modelMatrix;
    if(randomAngle == 0) {
        rotateDuck(-0.785);
        rotd = -0.785;
        } else {
        rotateDuck(0.785);
        rotd = 0.785;
        }

    this->h = h;
    this->w = w;

    this->lives = lives;
    this->bullets = bullets;
    this->score = score;
    *bullets = 3;
}

duck::~duck()
{
}

void duck::createMesh(const std::string &name, glm::vec3 leftBottomCorner, float length, bool fill) {
    glm::vec3 corner = leftBottomCorner;
    glm::vec3 red((float)(random() % 20 + 55), 0, 0);
    glm::vec3 green(0, (float)(random() % 20 + 70)/100, 0);

    glm::vec3 yellow(1, 1, 0);

    std::vector<VertexFormat> vertices;

    // //add cioc
    vertices.push_back(VertexFormat(corner + glm::vec3(length*0.25, 2.6*length, 0), yellow));
    vertices.push_back(VertexFormat(corner + glm::vec3(length*0.75, 2.6*length, 0), yellow));
    vertices.push_back(VertexFormat(corner + glm::vec3(length*0.5, 3.3*length, 0), yellow));
    // //add head
    glm::vec3 center(length/2, 2.3f*length, 0);
    float r = length/2;
    vertices.push_back(VertexFormat(corner + center, green));
    for(int i = 0; i < 36; i++) {
        float x = r*sin(i*10*M_PI/180) + r;
        float y = r*cos(i*10*M_PI/180) + 2.3f*length;
        vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), green));
    }

    //add body
        vertices.push_back(VertexFormat(corner+ glm::vec3(length/2, 0, 0), red));
        vertices.push_back(VertexFormat(corner + glm::vec3(0, 2*length, 0), red));
        vertices.push_back(VertexFormat(corner + glm::vec3(length, 2*length, 0), red));

    std::vector<unsigned int> indices;
    //add cioc indeces
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    //add head indices
    for(int i = 0 ; i  < 35 ; i++) {
        indices.push_back(3);
        indices.push_back(i + 4);
        indices.push_back(i + 5);
    }
        indices.push_back(3);
        indices.push_back(39);
        indices.push_back(4);

    //add body indices
    indices.push_back(40);
    indices.push_back(41);
    indices.push_back(42);


    mesh = new Mesh(name);

    mesh->InitFromData(vertices, indices);

}

void duck::createWings(const std::string &name1, const std::string &name2, glm::vec3 leftBottomCorner, float length, bool fill) {
    glm::vec3 corner = leftBottomCorner;
    glm::vec3 green(0, (float)(random() % 50 + 40)/100, 0);

    std::vector<VertexFormat> vertices;
    std::vector<VertexFormat> vertices1;

    vertices1.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), green));
    vertices1.push_back(VertexFormat(corner + glm::vec3(0, length, 0), green));
    vertices1.push_back(VertexFormat(corner + glm::vec3(-length*0.5, length/2, 0), green));

    vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), green));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, length, 0), green));
    vertices.push_back(VertexFormat(corner + glm::vec3(length*0.5, length/2, 0), green));

    std::vector<unsigned int> indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    rWing = new Mesh(name1);
    rWing->InitFromData(vertices, indices);
    lWing = new Mesh(name2);
    lWing->InitFromData(vertices1, indices);

    collider = object2D::CreateSquare("collider", corner, 1.5*len, 2*len, green);

}

void duck::rotateDuck(float coef) {
    modelMatrix = modelMatrix*transform2D::Translate(len/2, 1.65*len);
    modelMatrix *= transform2D::Rotate(coef); 
    modelMatrix = modelMatrix*transform2D::Translate(-len/2, -1.65*len);  
    rotd += coef; 
}

void duck::move(float deltatime,float speed) {

    if(modelMatrix[2][1] >= this->h-50 ){
        if(timery > 400/speed) {
            timery = 0;
            int coef = 1;
            if(modelMatrix[2][0] > prevx) {
                coef = -1;
            }
            rotateDuck(coef * M_PI/2);
        }
    }
    if(modelMatrix[2][1] <= 200) {
        if(timery > 400/speed) {
            timery = 0;
        modelMatrix = modelMatrix*transform2D::Translate(len/2, 1.65*len);
        int coef = 1;
        if(modelMatrix[2][0] < prevx)
           coef = -1;
        rotateDuck(coef * M_PI/2);
        }
    
    }

    if(modelMatrix[2][0] >= this->w-50 ){
        if(timerx > 400/speed) {
            timerx = 0;
            int coef = 1;
            if(modelMatrix[2][1] < prevy)
                coef = -1;
            rotateDuck(coef * M_PI/2);
        }
    }
    if(modelMatrix[2][0] <= 50) {
        if(timerx > 400/speed) {
            timerx = 0;
            int coef = 1;
            if(modelMatrix[2][1] > prevy)
                coef = -1;
            rotateDuck(coef * M_PI/2);
        }
    }
    timerx += deltatime;
    timery += deltatime;

    prevx = modelMatrix[2][0];
    prevy = modelMatrix[2][1];

    modelMatrix *= transform2D::Translate(0, deltatime*speed);
    animate(deltatime);
}
void duck::animate(float deltatime) {
    if(rot >= 0.60 || rot <= -0.2) { 
        coef = -coef;
    }
    rot += coef*deltatime;
    modelMatrixLW = modelMatrix;
    modelMatrixRW  = modelMatrix;
    modelMatrixLW = modelMatrix*transform2D::Translate(15, 30);
    modelMatrixRW = modelMatrix*transform2D::Translate(50,30);

    modelMatrixLW *= transform2D::Translate(0, 55);
    modelMatrixLW *= transform2D::Rotate(rot);
    modelMatrixLW *= transform2D::Translate(0, -55);

    modelMatrixRW *= transform2D::Translate(0, 55);
    modelMatrixRW *= transform2D::Rotate(-rot);
    modelMatrixRW *= transform2D::Translate(0, -55);
}
void duck::fly(float deltatime) {
    ColliderMatrix = modelMatrix;
    ColliderMatrix *= transform2D::Translate(0.75*len, len);
    ColliderMatrix *= transform2D::Rotate(-rotd);
    ColliderMatrix*= transform2D::Translate(0.75*-len, -len);

    timer += deltatime;
    
    if(timer > 5 || escaped) {
        if(ColliderMatrix[2][1] < -200 || ColliderMatrix[2][1] > h + 200) {
            deleted = true;
            return;
        }
        timer = 0;
        if(!lifed) {
        *lives -= 1;
        *score -= 1;
        }
        lifed = true;
        state = 2;
        rotateDuck(-rotd);
    }
    if(dead) {
        timer = 0;
        state = 0;
        rotateDuck(-rotd);
        rotateDuck(M_PI);

        }
    if(state == 1) {
        move(deltatime, no_duck*40 + 400);
        return;
    }
    if(state == 0) {
        die(deltatime);
        return;
    }
    if(state == 2) 
        escape(deltatime);
}

void duck::escape(float deltatime) {
    modelMatrix *= transform2D::Translate(0, deltatime* 350);
    if(modelMatrix[2][1] >= h + 100) {
        this->deleted = true;
        return;
    }
    animate(deltatime);
}

void duck::die(float deltatime) {
    modelMatrix *= transform2D::Translate(0, deltatime* 350);
    if(modelMatrix[2][1] <= 50) {
        this->deleted = true;
        return;
    }
    animate(deltatime);
}
