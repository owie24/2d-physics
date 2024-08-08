#ifndef _OBJECT_
#define _OBJECT_

#include <vector>

#include "shape.h"
#include "edge.h"

using namespace std;

class Object;

class Component {
    public:
        /**
         * Struct represening all the information of a contact
         * point with the noraml angle of it and x and y as offsets 
         * from the origin of the object
         */
        struct ContactPoint {
            float normalAngle;
            float x;
            float y;
        };


        /**
         * Default constructor taking in a shape
         * setting offset at the centre of the object
         */
        Component(Shape shape, bool collision);
        /**
         * Constrcutor taking in a shape 
         * and offset from it from centre of object
         */
        Component(Shape shape, bool collision, float xOffset, float yOffset);
        /**
         * Deconstructor
         */
        ~Component();
        /**
         * Renders component taking the objects
         * centre as the position
         */
        void Render(int x, int y, float angle);
        /**
         * Gets the wight of the componet, by default
         * the density of the shape is defined by 1.0
         * calculates weight off of area of the shape
         * and the density of the component
         */
        double Weight();
        /**
         * Sets the offset of the component
         * Used to shift the offset from the origin of the object
         * relative to the centre of mass of the object
         */
        void SetData(float x, float y);
        void SetData(int x, int y);
        /**
         * Gets the current offset of the component
         */
        pair<int, int> GetOffset();
        /**
         * Checks the hitbox of the componet and compares 
         * it to the borders
         */
        bool BorderHitbox(float objectX, float objectY, float angleOffset);
        /**
         * Calculates all the contact points of a component with
         * the windows borders
         */
        void BorderContacts(Object *thisObject, int originX, int originY, float angle);

    private:
        /**
         * Constructs the hitboxes bounds whenever something
         * is changed about the component
         */
        void MakeHitbox();
        /**
         * Makes the vector of edges based on the shape
         */
        void MakeEdgeList();
        /**
         * Compares the edges to the borders and returns a
         * vector of pairs representing speeds to
         */

        Shape shape;
        int xOffset;
        int yOffset;
        bool collision;
        double density;

        float originAngle;
        float distanceToOrigin;

        float shapeAngle;

        pair<pair<int, int>, pair<int, int>> hitbox;
        vector<Edge> shapeEdges;
};

class Object {
    public:
        /**
         * Default object constructor
         */
        Object();
        /**
         * Object constructor taking in various metadata
         * and a vector of components that define it
         */
        Object(vector<Component> components, float x, float y, bool gravity);
        /**
         * Renders object
         */
        void Render();
        /**
         * Rotates object about its centre of mass
         * temp??
         */
        void Rotate(float angle);
        /**
         * Ticks object, updates velocity if affected by gravity
         * and moves position based off new velocity
         */
        void Tick();
        /**
         * Sets the velocity of the object
         */
        void SetVelocity(float xVel, float yVel);
        /**
         * Sets the the angular frequency of the object
         */
        void SetRotationSpeed(float speed);
        /**
         * Checks border collison by first comparing the hitbox
         * then calculating if actually hit and angle
         */
        void BorderCollision();
        /**
         * Checks for collision with other objects
         * and adjusts velocities 
         */
        void ObjectCollision(Object otherObject);
        /**
         * Returns the active velocity of the object
         */
        pair<float, float> GetVelocities();
        /**
         * Adds velocities to the current
         * objects velocities
         */
        void AddVelocity(float xVel, float yVel);
        /**
         * Sets the angular frequency of the object
         * in rads/s
         */
        void SetAngularFrequency(float freq);
        /**
         * Adds angular frequency to the current
         * frequency
         */
        void AddAngularFrequency(float freq);
        /**
         * Returns the objects moment of inertia
         */
        float GetMomentInertia();
        /**
         * Sets the position of the object
         */
        void SetPosition(float x, float y);


    private:
        /**
         * Calculates the centre of mass of the total object
         * based off of indiviudal components and weight as
         * an offset from the current origin
         */
        pair<int, int> Centre();

        vector<Component> components;
        int xOrigin;
        int yOrigin;
        double rotationAngle;

        float xVelocity;
        float yVelocity;
        float angularFrequency;
        float momentInertia;

        bool gravity;
        
        
};

void PollObjects(vector<Object*> objects);

#endif