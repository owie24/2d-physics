
#include "object.h"

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

Object::Object() {
    xOrigin = 0.0;
    yOrigin = 0.0;
    components.clear();
    rotationAngle = 0.0;
    xVelocity = 0.0;
    yVelocity = 0.0;
    angularFrequency = 0.0;
    momentInertia = 0.0;
    gravity = false;
}

Object::Object(vector<Component> components, float x, float y, bool gravity) {
    this->components = components;
    this->gravity = gravity;
    pair<int, int> objectOffset = Object::Centre();
    this->xOrigin = x*1000;
    this->yOrigin = y*1000;
    this->xVelocity = 0.0;
    this->yVelocity = 0.0;
    rotationAngle = 0.0;
    angularFrequency = 0.0;
    momentInertia = 0.0;
    pair<int, int> componentOffset;
    for (auto it = components.begin(); it != components.end(); it++) {
        componentOffset = it->GetOffset();
        it->SetData(componentOffset.first - objectOffset.first, componentOffset.second - objectOffset.second);
        momentInertia += (pow((componentOffset.first - objectOffset.first), 2) + pow((componentOffset.second - objectOffset.second), 2))*it->Weight();
    }
}

void Object::Render() {
    for (auto it = components.begin(); it != components.end(); it++) {
        it->Render(xOrigin, yOrigin, rotationAngle);
    }
}

pair<int, int> Object::Centre() {
    double mass = 0.0;
    int xSum = 0, ySum = 0;
    for (auto it = this->components.begin(); it != this->components.end(); it++) {
        xSum += it->GetOffset().first*it->Weight();
        ySum += it->GetOffset().second*it->Weight();
        mass += it->Weight();
    }
    return make_pair((xSum/mass), (ySum/mass));
}

void Object::Rotate(float angle) {
    this->rotationAngle = angle;
}

void Object::BorderCollision() {
    for (auto it = components.begin(); it != components.end(); it++) {
        if (it->BorderHitbox(xOrigin, yOrigin, rotationAngle)) {
            it->BorderContacts(this, xOrigin, yOrigin, rotationAngle);
        }
    }
}

void Object::Tick() {
    if (gravity) yVelocity += 1.0/FRAME_RATE*GRAVITY;
    xOrigin += xVelocity;
    yOrigin += yVelocity;
    rotationAngle += 1.0/FRAME_RATE*angularFrequency;

    this->Render();
    this->BorderCollision();
}

pair<float, float> Object::GetVelocities() {
    return make_pair(this->xVelocity, this->yVelocity);
}

void Object::SetVelocity(float xVel, float yVel) {
    this->xVelocity = xVel;
    this->yVelocity = yVel;
}

void Object::AddVelocity(float xVel, float yVel) {
    this->xVelocity += xVel;
    this->yVelocity += yVel;
}

void Object::SetAngularFrequency(float freq) {
    this->angularFrequency = freq;
}

void Object::AddAngularFrequency(float freq) {
    this->angularFrequency += freq;
}

float Object::GetMomentInertia() {
    return momentInertia;
}

void Object::SetPosition(float x, float y) {
    xOrigin = x;
    yOrigin = y;
}