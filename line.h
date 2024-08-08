#ifndef _LINE_
#define _LINE_

#include <utility>

using namespace std;

enum Region {outLeft, outEdge, outRight, inside};

template <class T>
struct vector3
{
    T x;
    T y;
    T z;

    vector3(T x, T y, T z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

class Line {
    public:
        Line();

        Line(int startX, int startY, int endX, int endY);

        pair<bool, pair<float, float>> Intersects(Line otherLine);


        double slope;
        double yInt;
        int minX, minY, maxX, maxY;
        bool undefined;
        pair<int, int> startP, endP;
};

vector3<int> CrossProduct(vector3<int> line1, vector3<int> line2);
vector3<float> CrossProduct(vector3<float> line1, vector3<float> line2);

int Magnitude(vector3<int> vec);
float Magnitude(vector3<float> vec);

bool WithinTriangle(pair<int, int> A, pair<int, int> B, pair<int, int> C, pair<int, int> P);

#endif