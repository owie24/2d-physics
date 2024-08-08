#ifndef _EDGE_
#define _EGDE_

#include <math.h>
#include "line.h"

using namespace std;

class Edge {
    public:
        struct ReturnStuff
        {
            bool result;
            float length;
            bool continueThis;
            bool continueOther;

            ReturnStuff(bool result, float length, bool continueThis, bool continueOther) {
                this->result = result;
                this->length = length;
                this->continueThis = continueThis;
                this->continueOther = continueOther;
            }

            ReturnStuff() {
                this->result = false;
                this->length = 0.0;
                this->continueThis = false;
                this->continueOther = false;
            }

            ReturnStuff const & operator= (ReturnStuff const & other) {
                this->result = other.result;
                this->length = other.length;
                this->continueThis = other.continueThis;
                this->continueOther = other.continueOther;
                return *this;
            }
        };
        

        Edge(int startX, int startY, int endX, int endY);

        Edge();

        Edge const & operator= (Edge const & other);

        void SetPosition(int x, int y, float angle);

        Line GetEdgeLine();

        ReturnStuff WithinEdge(Edge otherEdge);

        float NormalAngle();



        double GetSlopeAngle();
        double GetSlopeLength();
        pair<int, int> GetStartPosition();
        pair<int, int> GetEndPosition();

        /**
         * Checks for collision with another Edge
         * either via intersection or having a vertex
         * within the edges "fov"
         */
        

        double slopeAngle;
        double slopeLength;

        double startAngle;
        double startDistance;
        double endAngle;
        double endDistance;

        int originX;
        int originY;
        double rotationAngle;
};

float CalculateAngle(float startX, float startY, float endX, float endY);

#endif