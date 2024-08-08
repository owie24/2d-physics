
#include "object.h"

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

Component::Component(Shape shape, bool collision) {
    this->shape = shape;
    this->collision = collision;
    SetData(0, 0);
    density = 1.0;
    this->shapeAngle = shape.GetAngle();
    this->MakeEdgeList();
    this->MakeHitbox();
}

Component::Component(Shape shape, bool collision, float xOffset, float yOffset) {
    this->shape = shape;
    this->collision = collision;
    SetData(xOffset, yOffset);
    density = 1.0;
    this->shapeAngle = shape.GetAngle();
    this->MakeEdgeList();
    this->MakeHitbox();
}

Component::~Component() {
}

void Component::Render(int x, int y, float angle) {
    this->shape.Render((x + this->distanceToOrigin*cos(angle + this->originAngle)), (y + this->distanceToOrigin*sin(angle + this->originAngle)), angle);
}

double Component::Weight() {
    return shape.Area()*density;
}

void Component::SetData(float x, float y) {
    this->xOffset = x*1000;
    this->yOffset = y*1000;
    this->originAngle = CalculateAngle(0.0, 0.0, x, y);
    this->distanceToOrigin = sqrt(pow(x, 2) + pow(y, 2));
}

void Component::SetData(int x, int y) {
    this->xOffset = x;
    this->yOffset = y;
    this->originAngle = CalculateAngle(0.0, 0.0, x, y);
    this->distanceToOrigin = sqrt(pow(x, 2) + pow(y, 2));
}

pair<int, int> Component::GetOffset() {
    return make_pair(this->xOffset, this->yOffset);
}

void Component::MakeHitbox() {
    hitbox = make_pair(make_pair(-shape.GetScales().first*500, shape.GetScales().first*500), make_pair(-shape.GetScales().second*500, shape.GetScales().second*500));
}

bool Component::BorderHitbox(float originX, float originY, float angleOffset) {
    float minX = originX + distanceToOrigin*cos(originAngle + angleOffset) + hitbox.first.first, maxX = originX + distanceToOrigin*cos(originAngle + angleOffset) + hitbox.first.second, minY = originY + distanceToOrigin*sin(originAngle + angleOffset) + hitbox.second.first, maxY = originY + distanceToOrigin*sin(originAngle + angleOffset) + hitbox.second.second;
    return minX <= -1000 || maxX >= 1000 || minY <= -1000 || maxY >= 1000;
}

void Component::MakeEdgeList() {
    vector<pair<float, float>> vertices = this->shape.GetVertices();
    float startAngle, endAngle;
    float distance = 1.0;
    for (int i = 1; i < vertices.size() - 1; i++) {
        startAngle = CalculateAngle(0.0, 0.0, (vertices.at(i).first), (vertices.at(i).second));
        startAngle += this->shapeAngle;
        endAngle = CalculateAngle(0.0, 0.0, (vertices.at(i+1).first), (vertices.at(i+1).second));
        endAngle += this->shapeAngle;
        this->shapeEdges.push_back(Edge(this->shape.GetScales().first*distance*cos(startAngle)*500, this->shape.GetScales().second*distance*sin(startAngle)*500, this->shape.GetScales().first*distance*cos(endAngle)*500, this->shape.GetScales().second*distance*sin(endAngle)*500));
    }
}

void Component::BorderContacts(Object *thisObject, int originX, int originY, float angle) {
    Edge lowBorder(-1000, 1000, 1000, 1000);
    lowBorder.SetPosition(0, -2000, 0);
    Edge rightBorder(-1000, -1000, -1000, 1000);
    rightBorder.SetPosition(2000, 0, 0.0);
    Edge upperBorder(-1000, -1000, 1000, -1000);
    upperBorder.SetPosition(0, 2000, 0);
    Edge leftBorder(1000, 1000, 1000, -1000);
    leftBorder.SetPosition(-2000, 0, 0);

    cout << "New Check" << endl << endl;
    Edge::ReturnStuff result;
    vector<Edge> edges;
    vector<pair<pair<float, pair<float, float>>, float>> angleNPos;
    double length = 0.0, angleDir;
    Edge border;
    int num, finish;
    bool lapped, connected, inside;

    for (int side = 0; side < 4; side++) {
        length = 0.0;
        edges.clear();
        lapped = false;
        connected = false;
        inside = false;
        finish = 0;

        if (side == 0) border = lowBorder;
        else if (side == 2) border = leftBorder;
        else if (side == 1) border = upperBorder;
        else border = rightBorder;

        for (int i = 0; i < shapeEdges.size()*2 - finish; i++) { 
            if (i >= shapeEdges.size()) num = i - shapeEdges.size();
            else num = i;

            shapeEdges.at(num).SetPosition(originX + xOffset, originY + yOffset, angle);
            result = shapeEdges.at(num).WithinEdge(border);

            if (lapped) {
                if (result.result) {
                    if (result.continueThis && result.continueOther) {
                        if (!inside) edges.clear();
                        inside = true;
                        edges.push_back(shapeEdges.at(num));
                    }
                    else if (!inside) {
                        edges.push_back(shapeEdges.at(num));
                    }
                }
                if ((!result.result || (i+1) >= (shapeEdges.size()*2 - finish)) && !edges.empty()) {
                    angleDir = 0.0;
                    for (auto it = edges.begin(); it != edges.end(); it++) angleDir += it->NormalAngle();

                    angleDir = angleDir/edges.size();
                    //cout << "Angle: " << angleDir << endl;
                    angleNPos.push_back(make_pair(make_pair(angleDir, make_pair(0.0, 0.0)), border.NormalAngle()));
                    edges.clear();
                    inside = false;
                }
            }
            else if (!result.result) {
                finish = shapeEdges.size() - i;
                lapped = true;
            }
        }
    }

    if (!angleNPos.empty()) {
        angleDir = 0.0;
        float normalAngle = 0.0;
        cout << "Size: " << angleNPos.size() << endl;
        for (auto it = angleNPos.begin(); it != angleNPos.end(); it++) {
            angleDir += it->first.first;
            normalAngle += it->second;
        }

        angleDir = angleDir / angleNPos.size();
        normalAngle = normalAngle / angleNPos.size() + M_PI/2;

        
        float velocity = sqrt(pow(thisObject->GetVelocities().first, 2) + pow(thisObject->GetVelocities().second, 2)), velAngle = CalculateAngle(0.0, 0.0, thisObject->GetVelocities().first, thisObject->GetVelocities().second);

        float normalVel = Magnitude(CrossProduct(vector3<float>(cos(normalAngle), sin(normalAngle), 0.0f), vector3<float>(thisObject->GetVelocities().first, thisObject->GetVelocities().second, 0.0f)));
        vector3<float> unChnageVel = CrossProduct(vector3<float>(cos(normalAngle - M_PI/2), sin(normalAngle - M_PI/2), 0.0f), vector3<float>(thisObject->GetVelocities().first, thisObject->GetVelocities().second, 0.0f));
    
        thisObject->SetVelocity(unChnageVel.x + 1.05*normalVel*cos(angleDir), unChnageVel.y + 1.05*normalVel*sin(angleDir));
    }

    cout << endl;
}