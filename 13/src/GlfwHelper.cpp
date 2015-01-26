#include "GlfwHelper.h"

void GlfwHelper::loop(){
    do {
        openglHelper->render();
        glfwSwapBuffers(window);
        glfwPollEvents();   
    } while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                   glfwWindowShouldClose(window) == 0);
}



GlfwHelper::GlfwHelper() {
    if (!glfwInit()) {
        std::cerr << "GLFW baslatilamadi." << std::endl;
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->window = glfwCreateWindow( 1024, 768, "AB2015-3D-ornek-1", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "GLFW penceresi acilamadi, ekran karti surucunuz OpenGL 3.3 desteklemiyor olabilir.\n" );
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW baslatilamadi" << std::endl;
        return;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    

    openglHelper = new GlHelper();
    if ( openglHelper == NULL ) {
        glfwTerminate();
    }

}
