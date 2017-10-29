//
//  main.cpp
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 25/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#include "Vector3.h"
#include "Shader.h"
#include "Curve.h"

bool lbutton_down;
double XPOS, YPOS;

unsigned int HEIGHT = 600, WIDTH = 600;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Curve* bezierPointer = (Curve*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
        bezierPointer->increaseSubdivision();
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
        bezierPointer->decreaseSubdivision();
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        bezierPointer->showPolyline = !bezierPointer->showPolyline;
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    Curve* bezierPointer = (Curve*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        bezierPointer->dragingIndex = bezierPointer->getHoverControlPointIndex(XPOS, YPOS);
        lbutton_down = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        if(bezierPointer->dragingIndex == -1)
            bezierPointer->appendControlPoint(Vector3(XPOS, YPOS, 0.0));
        lbutton_down = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        bezierPointer->removeControlPoint();
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    XPOS = (xpos-(HEIGHT/2))/(HEIGHT/2);
    YPOS = -(ypos-(WIDTH/2))/(WIDTH/2);
}


int main(int argc, const char * argv[]) {
    // Initialise GLFW
    if(!glfwInit()){
        glfwTerminate();
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "Curves Visualizer", NULL, NULL);
    if(!window){
        glfwDestroyWindow(window);
        return -1;
    }
    // Event listener and MakeContextCurrent
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwMakeContextCurrent(window);
    
    std::cout << "\n\n\n\n\n\n\n\n\n\n==============PROGRAM STARTED==============\n";
    
    
    unsigned int shaderProgram = loadShader();
    Curve bezier(shaderProgram);
    glfwSetWindowUserPointer(window, &bezier);
    
    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        if(lbutton_down) {
            if(bezier.dragingIndex != -1)
                bezier.drag(bezier.dragingIndex, Vector3(XPOS, YPOS, 0.0));
        }else{
            bezier.dragingIndex = -1;
        }
        
        bezier.draw();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // End of program
    glfwTerminate();
    return 0;
}
