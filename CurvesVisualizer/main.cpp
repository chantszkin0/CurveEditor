//
//  main.cpp
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 25/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#ifdef _WIN32
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "Vector3.h"
#include "Shader.h"
#include "Geometry.h"
#include "Curve.h"

bool lbutton_down;
double XPOS, YPOS;

unsigned int HEIGHT = 600, WIDTH = 600;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Curve* EditableCurvePointer = (Curve*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
        EditableCurvePointer->increaseSubdivision();
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
        EditableCurvePointer->decreaseSubdivision();
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        EditableCurvePointer->showPolyline = !EditableCurvePointer->showPolyline;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        EditableCurvePointer->switchCurve();
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
        return 1;
    }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "Curves Visualizer", NULL, NULL);
    if(!window){
        glfwDestroyWindow(window);
        return 2;
    }
    // Event listener and MakeContextCurrent
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwMakeContextCurrent(window);

#ifdef _WIN32
	// GLEW
	if (glewInit() != GLEW_OK)
		std::cout << "ERROR::GLEW_INIT()\n";
#endif
    
    std::cout << "\n\n\n\n\n\n\n\n\n\n==============PROGRAM STARTED==============\n";
    
    
    unsigned int shaderProgram = loadShader();
    Curve EditableCurve(shaderProgram);
    glfwSetWindowUserPointer(window, &EditableCurve);
    
    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        if(lbutton_down) {
            if(EditableCurve.dragingIndex != -1)
                EditableCurve.drag(EditableCurve.dragingIndex, Vector3(XPOS, YPOS, 0.0));
        }else{
            EditableCurve.dragingIndex = -1;
        }
        
        EditableCurve.draw();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // End of program
    glfwTerminate();
    return 0;
}
