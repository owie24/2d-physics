#include "shape.h"

std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> sidesToVAO;
std::unordered_map<unsigned int, std::vector<pair<float, float>>> shapeVertices;

Shape::Shape() {
    xScale = 1.0;
    yScale = 1.0;
    color = RGBA();
    sides = -1;
}

Shape::Shape(unsigned int sides, float width, float height, float angle, RGBA *color) {
    if (sidesToVAO[sides] == std::pair<unsigned int, unsigned>()) makeShapeVAO(sides);
    xScale = width;
    yScale = height;
    if (color == NULL) this->color = RGBA();
    else this->color = *color;
    this->sides = sides;
    this->angle = angle;
    std::cout << "Angle: " << this->angle << std::endl;
}

Shape::Shape(unsigned int sides, float radius, float angle, RGBA *color) {
    if (sidesToVAO[sides] == std::pair<unsigned int, unsigned>()) makeShapeVAO(sides);
    xScale = radius;
    yScale = radius;
    if (color == NULL) this->color = RGBA();
    else this->color = *color;
    this->sides = sides;
    this->angle = angle;
    std::cout << "Angle: " << this->angle << std::endl;
}

void Shape::makeShapeVAO(unsigned int sides) {
    unsigned int VAO, VBO;
    float twoPi = M_PI*2;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::vector<float> verticies;
    std::vector<pair<float, float>> vertexData;
    verticies.push_back(0.0f);
    verticies.push_back(0.0f);
    vertexData.push_back(make_pair(verticies.at(verticies.size()- 2), verticies.at(verticies.size()- 1)));
    for (unsigned i = 0; i <= sides; i++) {
        verticies.push_back(0.5*cos(i*twoPi/sides));
        verticies.push_back(0.5*sin(i*twoPi/sides));
        vertexData.push_back(make_pair(verticies.at(verticies.size()- 2), verticies.at(verticies.size()- 1)));
    } 

    glBufferData(GL_ARRAY_BUFFER, verticies.size()*4, verticies.data(), GL_DYNAMIC_DRAW);
    sidesToVAO[sides] = std::make_pair(VAO, VBO);
    shapeVertices[sides] = vertexData;
}

Shape const & Shape::operator=(Shape const &other) {
    if (this != &other) {
        if (sidesToVAO[other.sides] == std::pair<unsigned int, unsigned>()) makeShapeVAO(other.sides);
        this->yScale = other.yScale;
        this->xScale = other.xScale;
        this->color = other.color;
        this->sides = other.sides;
        this->angle = other.angle;
    }
    return *this;
}

void Shape::Render(int x, int y, float angle) {
    glBindVertexArray(sidesToVAO[this->sides].first);
    ourShader->use();
    ourShader->setVec4("newColor", glm::vec4(color.r, color.g, color.b, color.a));
    ourShader->setVec2("position", glm::vec2((float) x/1000.0, (float) y/1000.0));
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, -(this->angle), glm::vec3(0, 0, 1.0));
    transform = glm::scale(transform, glm::vec3(xScale, yScale, 0.0));
    transform = glm::rotate(transform, -(angle), glm::vec3(0, 0, 1.0));
    ourShader->setMat4("transform", transform);

    glDrawArrays(GL_TRIANGLE_FAN, 0, (sides + 2)); 
}

double Shape::Area() {
    double area = 0.0, distance, twoPi = M_PI*2;
    for (unsigned i = 0; i < this->sides; i++) {
        distance = sqrt(pow(xScale*(0.5*cos((i + 1)*twoPi/sides) - 0.5*cos(i*twoPi/sides)), 2) + pow(yScale*(0.5*sin((i + 1)*twoPi/sides) - 0.5*sin(i*twoPi/sides)), 2));
        area += sqrt((pow(xScale*0.5*cos(i*twoPi/sides), 2) + pow(yScale*0.5*sin(i*twoPi/sides), 2)) - (pow((distance/2), 2)))*distance*0.5;
    }
    return area;
}

pair<float, float> Shape::GetScales() {
    return make_pair(xScale, yScale);
}

vector<pair<float, float>> Shape::GetVertices() {
    return shapeVertices[this->sides];
}

float Shape::GetAngle() {
    return this->angle;
}