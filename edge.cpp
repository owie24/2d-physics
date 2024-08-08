#include "edge.h"
#include <iostream>


Edge::Edge(int startX, int startY, int endX, int endY) {
    slopeLength = sqrt(pow((double) (endX - startX), 2) + pow((double) (endY - startY), 2));
    slopeAngle = CalculateAngle(startX, startY, endX, endY);
    startAngle = CalculateAngle(0.0, 0.0, startX, startY);
    startDistance = sqrt(pow((double) startX, 2) + pow((double) startY, 2));
    endAngle = CalculateAngle(0.0, 0.0, endX, endY);
    endDistance = sqrt(pow((double) endX, 2) + pow((double) endY, 2));
    originX = 0.0;
    originY = 0.0;
    rotationAngle = 0.0; 

    //cout << "Start Dist.: " << startDistance << endl;
}

void Edge::SetPosition(int x, int y, float angle) {
    this->originX = x;
    this->originY = y;
    this->rotationAngle = angle;
}

double Edge::GetSlopeAngle() {
    return this->slopeAngle + rotationAngle;
}

double Edge::GetSlopeLength() {
    return this->slopeLength;
}

pair<int, int> Edge::GetStartPosition() {
    return make_pair((originX + startDistance*cos(startAngle + rotationAngle)), (originY + startDistance*sin(startAngle + rotationAngle)));
}

pair<int, int> Edge::GetEndPosition() {
    return make_pair((originX + endDistance*cos(endAngle + rotationAngle)), (originY + endDistance*sin(endAngle + rotationAngle)));
}

float CalculateAngle(float startX, float startY, float endX, float endY) {
    if (startX == endX && startY == endY) return 0.0;
    else {
        float x = endX - startX, y = endY - startY;
        if (abs(x) < 0.000001) {
            if (y > 0) return M_PI/2;
            else return 3*M_PI/2;
        }
        else {
            if (y >= 0 && x > 0) return tanh(y/x);
            else if (y >= 0) return M_PI - tanh(y/abs(x));
            else if (x > 0) return 2*M_PI - tanh(abs(y)/x);
            else return M_PI + tanh(abs(y)/abs(x));
        }
    }
}

Line Edge::GetEdgeLine() {
    return Line((originX + startDistance*cos(startAngle + rotationAngle)), (originY + startDistance*sin(startAngle + rotationAngle)), (originX + endDistance*cos(endAngle + rotationAngle)), (originY + endDistance*sin(endAngle + rotationAngle)));
}

Edge::ReturnStuff Edge::WithinEdge(Edge otherEdge) {
    Line otherStartLine(otherEdge.originX, otherEdge.originY, otherEdge.originX + otherEdge.startDistance*cos(otherEdge.startAngle + otherEdge.rotationAngle), otherEdge.originY + otherEdge.startDistance*sin(otherEdge.startAngle + rotationAngle));
    Line otherEdgeLine(otherEdge.originX + otherEdge.startDistance*cos(otherEdge.startAngle + otherEdge.rotationAngle), otherEdge.originY + otherEdge.startDistance*sin(otherEdge.startAngle + rotationAngle), otherEdge.originX + otherEdge.endDistance*cos(otherEdge.endAngle + otherEdge.rotationAngle), otherEdge.originY + otherEdge.endDistance*sin(otherEdge.endAngle + otherEdge.rotationAngle));
    Line otherEndLine(otherEdge.originX + otherEdge.endDistance*cos(otherEdge.endAngle + otherEdge.rotationAngle), otherEdge.originY + otherEdge.endDistance*sin(otherEdge.endAngle + otherEdge.rotationAngle), otherEdge.originX, otherEdge.originY);
    Line thisEdgeLine(originX + startDistance*cos(startAngle + rotationAngle), originY + startDistance*sin(startAngle + rotationAngle), originX + endDistance*cos(endAngle + rotationAngle), originY + endDistance*sin(endAngle + rotationAngle));
    pair<bool, pair<float, float>> resultStart = thisEdgeLine.Intersects(otherStartLine), resultEgde = thisEdgeLine.Intersects(otherEdgeLine), resultEnd = thisEdgeLine.Intersects(otherEndLine);
    
    if (WithinTriangle(otherStartLine.startP, otherStartLine.endP, otherEndLine.startP, thisEdgeLine.startP) && WithinTriangle(otherStartLine.startP, otherStartLine.endP, otherEndLine.startP, thisEdgeLine.endP)) return ReturnStuff(true, sqrt(pow((thisEdgeLine.endP.first - thisEdgeLine.startP.first), 2) + pow((thisEdgeLine.endP.second - thisEdgeLine.startP.second), 2)), true, true);
    else if (WithinTriangle(otherStartLine.startP, otherStartLine.endP, otherEndLine.startP, thisEdgeLine.startP)) {
        if (resultStart.first) return ReturnStuff(true, sqrt(pow((thisEdgeLine.startP.first - resultStart.second.first), 2) + pow((thisEdgeLine.startP.second - resultStart.second.second), 2)), true, false);
        else if (resultEgde.first) return ReturnStuff(true, sqrt(pow((thisEdgeLine.startP.first - resultEgde.second.first), 2) + pow((thisEdgeLine.startP.second - resultEgde.second.second), 2)), false, false);
        else return ReturnStuff(true, sqrt(pow((thisEdgeLine.startP.first - resultEnd.second.first), 2) + pow((thisEdgeLine.startP.second - resultEnd.second.second), 2)), false, true);
    }
    else if (WithinTriangle(otherStartLine.startP, otherStartLine.endP, otherEndLine.startP, thisEdgeLine.endP)) {
        if (resultStart.first) return ReturnStuff(true, sqrt(pow((thisEdgeLine.endP.first - resultStart.second.first), 2) + pow((thisEdgeLine.endP.second - resultStart.second.second), 2)), true, false);
        else if (resultEgde.first) return ReturnStuff(true, sqrt(pow((thisEdgeLine.endP.first - resultEgde.second.first), 2) + pow((thisEdgeLine.endP.second - resultEgde.second.second), 2)), true, false);
        else return ReturnStuff(true, sqrt(pow((thisEdgeLine.endP.first - resultEnd.second.first), 2) + pow((thisEdgeLine.endP.second - resultEnd.second.second), 2)), true, false);
    }
    else if ((resultStart.first && resultEgde.first) || (resultStart.first && resultEnd.first) || (resultEgde.first && resultEnd.first)) {
        if (resultStart.first && resultEgde.first) return ReturnStuff(true, sqrt(pow((resultStart.second.first - resultEgde.second.first), 2) + pow((resultStart.second.second - resultEgde.second.second), 2)), false, false);
        else if (resultStart.first && resultEnd.first) return ReturnStuff(true, sqrt(pow((resultStart.second.first - resultEnd.second.first), 2) + pow((resultStart.second.second - resultEnd.second.second), 2)), false, true);
        else return ReturnStuff(true, sqrt(pow((resultEgde.second.first - resultEnd.second.first), 2) + pow((resultEgde.second.second - resultEnd.second.second), 2)), false, true);
    } 
    else return ReturnStuff(false, 0, false, false);
} 


float Edge::NormalAngle() {
    float angle = fmod((this->slopeAngle + rotationAngle), 2*M_PI) + M_PI/2;
    if (angle < 0) angle = 2*M_PI + angle;
    else if (angle > 2*M_PI) angle = angle - 2*M_PI;
    if (abs(angle) < 0.01) return 2*M_PI;
    else return angle;
}

Edge const & Edge::operator= (Edge const & other) {
    this->slopeAngle = other.slopeAngle;
    this->slopeLength = other.slopeLength;

    this->startAngle = other.startAngle;
    this->startDistance = other.startDistance;
    this->endAngle = other.endAngle;
    this->endDistance = other.endDistance;

    this->originX = other.originX;
    this->originY = other.originY;
    this->rotationAngle = other.rotationAngle;
    return *this;
}

Edge::Edge() {}