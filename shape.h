#ifndef _SHAPE_
#define _SHAPE_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "libs/RGBA.h"
#include <utility>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <iostream>
#include "shader.h"
#include <unistd.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern RGBA *RED;
extern RGBA *ORANGE;
extern RGBA *YELLOW;
extern RGBA *GREEN;
extern RGBA *BLUE;
extern RGBA *INDIGO;
extern RGBA *VIOLET;

extern std::unordered_map<unsigned int, std::pair<unsigned int, unsigned int>> sidesToVAO;
extern std::unordered_map<unsigned int, std::vector<pair<float, float>>> shapeVertices;
extern Shader *ourShader;
extern GLFWwindow *window;
extern glm::vec2 window_dims;

extern float GRAVITY;
extern unsigned int WINDOW_WIDTH;
extern unsigned int WINDOW_HEIGHT;
extern float FRAME_RATE;

class Shape {
    public:
        /**
         * Default constrctor
         */
        Shape();
        /**
         * Makes centred shape of declared sides and
         * radius
         */
        Shape(unsigned int sides, float radius, float angle, RGBA *color);
        /**
         * Makes centred shape of declared sides and 
         * height and width
         */
        Shape(unsigned int sides, float width, float height, float angle, RGBA *color);
        /**
         * Renders shape on screen
         */
        void Render(int x, int y, float angle);
        /**
         * Calculates the area of the shape in pixels
         * 
         * Note - same shape will vary in area depening on window dimensions
         */
        double Area();
        /**
         * Returns a pair of values representing the
         * x and y scales of the shape
         */
        pair<float, float> GetScales();
        /**
         * Gets and returns the vector of vertex data
         * unique to this normalized shape
         */
        vector<pair<float, float>> GetVertices();
        /**
         * Returns the angle of the shape
         */
        float GetAngle();
        

        Shape const & operator=(Shape const &other);

    
    private:
        /**
         * Makes object buffer for shape if it does not
         * already exists
         */
        void makeShapeVAO(unsigned int sides);

        
        RGBA color;
        unsigned int sides;
        float xScale;
        float yScale;
        float angle;
        //Texture texture;

        
};
#endif