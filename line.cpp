#include "line.h"
#include <math.h>
#include <iostream>

Line::Line() {
    slope = 0.0;
    undefined = true;
    minX = 0.0;
    minY = 0.0;
    maxX = 0.0;
    maxY = 0.0;
    yInt = 0.0; 
}


Line::Line(int startX, int startY, int endX, int endY) {
    minX = min(startX, endX);
    minY = min(startY, endY);
    maxX = max(startX, endX);
    maxY = max(startY, endY);
    startP = make_pair(startX, startY);
    endP = make_pair(endX, endY);

    if (abs(startX - endX) < 0.01) {
        undefined = true;
        slope = MAXFLOAT;
        yInt = 0;
    }
    else {
        undefined = false;
        if (startX <= endX) {
            //cout << "HERE1" << endl;
            slope = (endY - startY)/(endX - startX);
        }
        else {
            //cout << "HERE2" << endl;
            slope = (startY - endY)/(startX - endX);
        }
        yInt = startY - slope*startX;
    }
}

pair<bool, pair<float, float>> Line::Intersects(Line otherLine) {
    if (this->undefined || otherLine.undefined) {
        if (this->undefined && otherLine.undefined) return make_pair(false, make_pair(0,0));
        else if (this->undefined) {
            float yMin = otherLine.slope*this->minX + otherLine.yInt, yMax = otherLine.slope*this->maxX + otherLine.yInt;
            if (yMax < this->minY - 0.05/(abs(otherLine.slope) + abs(this->slope)) || yMin > this->maxY + 0.05/(abs(otherLine.slope) + abs(this->slope)) || this->maxX < otherLine.minX - 0.05/(abs(otherLine.slope) + abs(this->slope)) || this->minX > otherLine.maxX + 0.05/(abs(otherLine.slope) + abs(this->slope))) return make_pair(false, make_pair(0,0));
            else return make_pair(true, make_pair(this->minX, yMin));
        }
        else {
            float yMin = this->slope*otherLine.minX + this->yInt, yMax = this->slope*otherLine.maxX + this->yInt;
            if (yMax < otherLine.minY - 0.05/(abs(otherLine.slope) + abs(this->slope)) || yMin > otherLine.maxY + 0.05/(abs(otherLine.slope) + abs(this->slope)) || otherLine.maxX < this->minX - 0.05/(abs(otherLine.slope) + abs(this->slope)) || otherLine.minX > this->maxX  + 0.05/(abs(otherLine.slope) + abs(this->slope))) return make_pair(false, make_pair(0,0));
            else return make_pair(true, make_pair(otherLine.minX, yMin));
        }
    }
    else {
        if (this->slope == otherLine.slope) return make_pair(false, make_pair(0,0));
        else {
            double slopeSub, interceptSubs;
            slopeSub = this->slope - otherLine.slope;
            interceptSubs = otherLine.yInt - this->yInt;
            double x = interceptSubs/slopeSub;
            //cout << "Slope: " << this->slope << ", MinX: " << this->minX << ", MinY: " << this->minY << ", MaxX: " << this->maxX << ", MaxY: " << this->maxY << ", This Y-Int: " << this->yInt << ", Other Y-Int: " << otherLine.yInt << ", X: " << x << endl;
            if (x < this->minX - 0.3 || x < otherLine.minX - 0.3 || x > this->maxX + 0.3 || x > otherLine.maxX + 0.3) return make_pair(false, make_pair(0,0));
            else return make_pair(true, make_pair(x, (x*otherLine.slope + otherLine.yInt)));
        }
    }
}

vector3<int> CrossProduct(vector3<int> line1, vector3<int> line2) {
    int x = line1.y*line2.z - line1.z*line2.y;
    int y = -(line1.x*line2.z - line1.z*line2.x);
    int z = (line1.x*line2.y) - line1.y*line2.x;
    return vector3<int>(x, y, z);
}

vector3<float> CrossProduct(vector3<float> line1, vector3<float> line2) {
    float x = line1.y*line2.z - line1.z*line2.y;
    float y = -(line1.x*line2.z - line1.z*line2.x);
    float z = (line1.x*line2.y) - line1.y*line2.x;
    return vector3<float>(x, y, z);
}

int Magnitude(vector3<int> vec) {
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

float Magnitude(vector3<float> vec) {
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}


bool WithinTriangle(pair<int, int> A, pair<int, int> B, pair<int, int> C, pair<int, int> P) {
    int ABC = 0.5*Magnitude(CrossProduct(vector3<int>((B.first - A.first), (B.second - A.second), 0.0), vector3<int>((C.first - A.first), (C.second - A.second), 0.0)));
    int PAB = 0.5*Magnitude(CrossProduct(vector3<int>((P.first - A.first), (P.second - A.second), 0.0), vector3<int>((P.first - B.first), (P.second - B.second), 0.0)));
    int PBC = 0.5*Magnitude(CrossProduct(vector3<int>((P.first - B.first), (P.second - B.second), 0.0), vector3<int>((P.first - C.first), (P.second - C.second), 0.0)));
    int PAC = 0.5*Magnitude(CrossProduct(vector3<int>((P.first - A.first), (P.second - A.second), 0.0), vector3<int>((P.first - C.first), (P.second - C.second), 0.0)));
    //cout << "A: " << A.first << ", " << A.second << ", B: " << B.first << ", " << B.second << ", C: " << C.first << ", " << C.second << ", P: " << P.first << ", " << P.second << endl; 
    //cout << "ABC:: " << ABC << ", OTHER TRI:: " << (PAB + PBC + PAC) << endl;
    return (PAB + PBC + PAC) == ABC;
}