#include "lab_m1/Tema1/UI.h"
#include "lab_m1/Tema1/transform2D.h"

using namespace m1;

UI::UI(float x, float y)
{
    rezX = x;
    rezY = y;
    generateHearts();
    generateBullets();
    generateScore();
    generateButtons(true);
    generateButtons(false);
}

UI::~UI()
{
}

void UI::generateHearts() {
    glm::vec3 red(1, 0, 0);
    glm::vec3 corner = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices;
    vertices.push_back(VertexFormat(corner + glm::vec3(lenhearth/2, 0, 0), red));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, lenhearth, 0), red));
    vertices.push_back(VertexFormat(corner + glm::vec3(lenhearth/2, lenhearth*2/3, 0), red));
    vertices.push_back(VertexFormat(corner + glm::vec3(lenhearth, lenhearth, 0), red));
    std::vector<unsigned int> indices;
    //add cioc indeces
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);  
    hearth = new Mesh("hearth");
    hearth->InitFromData(vertices, indices);

}

void UI::generateBullets() {
    glm::vec3 grey(0.78, 0.74, 0);
    glm::vec3 corner = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), grey));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, lenbullet, 0), grey));
    vertices.push_back(VertexFormat(corner + glm::vec3(lenbullet*2/3, lenbullet, 0), grey));
    vertices.push_back(VertexFormat(corner + glm::vec3(lenbullet*2/3, 0, 0), grey));
    vertices.push_back(VertexFormat(corner + glm::vec3(lenbullet/3, lenbullet*3/2, 0), grey));
   
    std::vector<unsigned int> indices;
    //add cioc indeces
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);  
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(4);    
    bullet = new Mesh("bullet");
    bullet->InitFromData(vertices, indices);

}

void UI::generateScore() {
    glm::vec3 blue(0, 0.65, 1);
    glm::vec3 corner = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), blue));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, lenscore, 0), blue));
    vertices.push_back(VertexFormat(corner + glm::vec3(lenscore/2, lenscore, 0), blue));
    vertices.push_back(VertexFormat(corner + glm::vec3(lenscore/2, 0, 0), blue));
   
    std::vector<unsigned int> indices;
    //add cioc indeces
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);  
   
    score = new Mesh("score");
    score->InitFromData(vertices, indices);

}
void UI::generateButtons(bool end) {
    glm::vec3 green(0, 1, 0);
    if(end)
        green = glm::vec3(1, 0, 0);
    glm::vec3 corner = glm::vec3(0, 0, 0);
    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), green));
    vertices.push_back(VertexFormat(corner + glm::vec3(0, buttonlen/6, 0), green));
    vertices.push_back(VertexFormat(corner + glm::vec3(buttonlen, buttonlen/6, 0), green));
    vertices.push_back(VertexFormat(corner + glm::vec3(buttonlen, 0, 0), green));
   
    std::vector<unsigned int> indices;
    //add cioc indeces
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);  

    if(end) {
        buttonr = new Mesh("redbutton");
        buttonr->InitFromData(vertices, indices);
    } else {
        buttong = new Mesh("greenbutton");
        buttong->InitFromData(vertices, indices);
    }
}
glm::mat3 UI::hearthpoz(int no) {
    glm::mat3 poz = glm::mat3(1);
    poz *= transform2D::Translate(10 + 10*no + lenhearth * no, rezY - 100);
    return poz;
}
glm::mat3 UI::bulletpoz(int no) {
    glm::mat3 poz = glm::mat3(1);
    poz *= transform2D::Translate(10 + 10*no + lenbullet * no, rezY - 170);
    return poz;
}
glm::mat3 UI::scorepoz(int no) {
    glm::mat3 poz = glm::mat3(1);
    poz *= transform2D::Translate(rezX - 287.5 + lenscore/2 * (no - 1), rezY - 100);
    return poz;
}