#ifndef GLUT_HELPER_H
#define GLUT_HELPER_H

//order is important
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "GlHelper.h"
#include <stdio.h>

class GlfwHelper {

private:
    GLFWwindow* window;
    GlHelper* openglHelper;

public:
    GlfwHelper();
    void loop();
};



#endif
