#include "GlfwHelper.h"

void GlfwHelper::loop() {
    do {
        openglHelper->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
              glfwWindowShouldClose(window) == 0);
}

GlHelper * GlfwHelper::getGlHelper() {
    return openglHelper;
}

static void resize_callback(GLFWwindow* window, int height, int width){
    GlfwHelper* glfwHelper = static_cast<GlfwHelper*>(glfwGetWindowUserPointer(
                                 window));
    glfwHelper->getGlHelper()->reshape(height, width);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    GlfwHelper* glfwHelper = static_cast<GlfwHelper*>(glfwGetWindowUserPointer(
                                 window));
    glfwHelper->getGlHelper()->cameraMove(xpos,ypos);
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    GlfwHelper* glfwHelper = static_cast<GlfwHelper*>(glfwGetWindowUserPointer(
                                 window));

    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_UP:
            glfwHelper->getGlHelper()->processInput(GlHelper::UP);
            break;

        case GLFW_KEY_DOWN:
            glfwHelper->getGlHelper()->processInput(GlHelper::DOWN);
            break;

        case GLFW_KEY_LEFT:
            glfwHelper->getGlHelper()->processInput(GlHelper::LEFT);
            break;

        case GLFW_KEY_RIGHT:
            glfwHelper->getGlHelper()->processInput(GlHelper::RIGHT);
            break;
        }
    }

}


GlfwHelper::GlfwHelper(int width, int height) {
    if (!glfwInit()) {
        std::cerr << "GLFW baslatilamadi." << std::endl;
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::string title =  "AB2016-3D-";
    title += BIN_NAME;
    this->window = glfwCreateWindow( width, height, title.c_str(), NULL, NULL);

    if( window == NULL ) {
        fprintf( stderr, "GLFW penceresi acilamadi, ekran karti surucunuz OpenGL 3.3 desteklemiyor olabilir.\n" );
        glfwTerminate();
        return;
    }

    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW baslatilamadi" << std::endl;
        return;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window, key_callback);

    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    openglHelper = new GlHelper(width,height);

    if (openglHelper == NULL) {
        glfwTerminate();
    }

}
