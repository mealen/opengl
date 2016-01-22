#include "GlfwHelper.h"

int main(int argc, char** argv)
{
    GlfwHelper glfwHelper(1024,768);
    glfwHelper.loop();

    return 0;
}
