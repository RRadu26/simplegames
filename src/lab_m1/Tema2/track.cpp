#include "lab_m1/Tema2/track.h"
#include "components/simple_scene.h"
#include <iostream>
#include <fstream>
using namespace m1;
using namespace std;
track::track(vector<glm::vec3> initialPoints) {
    init(initialPoints);
}
void track::init(vector<glm::vec3> initialPoints) {

    for(int i = 0 ; i < initialPoints.size() - 2; i += 2) {
        initialPoints.insert(initialPoints.begin() + i + 1, initialPoints[i] + (initialPoints[i+1] - initialPoints[i])*0.5f);
    }
    for(int i = 0 ; i < initialPoints.size() - 2; i += 2) {
        initialPoints.insert(initialPoints.begin() + i + 1, initialPoints[i] + (initialPoints[i+1] - initialPoints[i])*0.5f);
    }
    for(int i = 0 ; i < initialPoints.size() - 2; i += 2) {
        initialPoints.insert(initialPoints.begin() + i + 1, initialPoints[i] + (initialPoints[i+1] - initialPoints[i])*0.5f);
    }
    for(int i = 0 ; i < initialPoints.size() - 1; i++) {
        glm::vec3 D = initialPoints[i + 1] - initialPoints[i];
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        P = glm::normalize(P);
        glm::vec3 sP = initialPoints[i] + h*P;
        glm::vec3 iP = initialPoints[i] - h*P;
        vertices.push_back(VertexFormat(sP, glm::vec3(0, 1, 0), glm::vec3(0.3, 0.3, 0.35)));
        vertices.push_back(VertexFormat(iP, glm::vec3(0, 1, 0), glm::vec3(0.35, 0.3, 0.3)));
    }

    glm::vec3 D = initialPoints[0] - initialPoints[initialPoints.size() - 1];
    glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        P = glm::normalize(P);
    glm::vec3 sP = initialPoints[initialPoints.size() - 1] + h*P;
    glm::vec3 iP = initialPoints[initialPoints.size() - 1] - h*P;
    vertices.push_back(VertexFormat(sP, glm::vec3(0, 1, 0), glm::vec3(0.4, 0.4, 0.4)));
    vertices.push_back(VertexFormat(iP, glm::vec3(0, 1, 0), glm::vec3(0.4, 0.4, 0.4)));

    for(int i= 0; i < vertices.size() - 2; i+= 2) {
        indices.push_back(i+1);
        indices.push_back(i+3);
        indices.push_back(i);
        indices.push_back(i+3);
        indices.push_back(i+2);
        indices.push_back(i);
    }
    int i = vertices.size() - 2;
    indices.push_back(i+1);
    indices.push_back(1);
    indices.push_back(i);
    indices.push_back(1);
    indices.push_back(0);
    indices.push_back(i); 

}