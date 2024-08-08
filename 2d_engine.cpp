#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "shader.h"
#include "libs/RGBA.h"
#include <math.h>
#include "libs/image.h"
#include <unordered_map>
#include "object.h"
#include <unistd.h> 
#include <list>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Shader *ourShader;
GLFWwindow* window;

glm::vec2 window_dims;
unsigned int WINDOW_WIDTH;
unsigned int WINDOW_HEIGHT;
bool holdingObj;

float FRAME_RATE = 32.0;
float GRAVITY = -15;
Object *object;

RGBA *RED;
RGBA *ORANGE;
RGBA *YELLOW;
RGBA *GREEN;
RGBA *BLUE;
RGBA *INDIGO;
RGBA *VIOLET;


int main() {
    WINDOW_WIDTH = 600;
    WINDOW_HEIGHT = 600;
    holdingObj = false;
    RED = new RGBA(255, 0, 0);
    ORANGE = new RGBA(255, 127, 0);
    YELLOW = new RGBA(255, 255, 0);
    GREEN = new RGBA(0, 255, 0);
    BLUE = new RGBA(0, 0, 255);
    INDIGO = new RGBA(75, 0, 130);
    VIOLET = new RGBA(148, 0, 211);
    window_dims = glm::vec2(WINDOW_WIDTH >> 1, WINDOW_HEIGHT >> 1);

    if (!glfwInit()) {
        std::cout << "Failed to initialize window\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "2D Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    
    ourShader = new Shader("shaders/vertexShader.vs", "shaders/fragShader.fs");
    glClearColor(128.0/255.0, 128.0/255.0, 128.0/255.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    vector<Component> components;
    components.push_back(Component(Shape(4, 1.0, M_PI/4, YELLOW), false, 0, -0.1));
    //components.push_back(Component(Shape(4, 0.4, 0.25, M_PI/4, RED), false, -0.1, 0.2));
    //components.push_back(Component(Shape(4, 0.25, M_PI/4, BLUE), false, 0.1, 0.2));
    object = new Object(components, 0, 0.5, true);

    Shape triangle(4, 1.0, 1.0, BLUE);
    //triangle.SetAngle(M_PI/4);
    cout << "Area: " << triangle.Area() << endl;

    

    list<int> newList;
    //Shape test(100, 50, VIOLET);
    int i = 0; 

    double time = glfwGetTime(), newTime;
    
    while(!glfwWindowShouldClose(window)) {

        processInput(window);
        glClearColor(128.0/255.0, 128.0/255.0, 128.0/255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        object->Tick();
        i++;
        if (i > 1200) i = 0;
        //triangle.Render(400, 400, M_PI/4);


        glfwSwapBuffers(window);
        glfwPollEvents();   

        newTime = glfwGetTime();
        if ((newTime - time) < 1.0/FRAME_RATE)   std::this_thread::sleep_for(chrono::microseconds((int) ((1.0/FRAME_RATE - (newTime - time))*pow(10, 6))));
        time = glfwGetTime();
    }


    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if (holdingObj) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        object->SetPosition((x - (WINDOW_WIDTH >> 1))/(WINDOW_WIDTH >> 1), -(y- (WINDOW_HEIGHT >> 1))/(WINDOW_HEIGHT >> 1));
    }
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) holdingObj = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) holdingObj = false;
}