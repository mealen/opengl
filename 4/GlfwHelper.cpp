//gpl sheyi
#include "GlfwHelper.h"

void GlfwHelper::loop() {
	do {
		openglHelper->render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
			&& glfwWindowShouldClose(window) == 0);
}

GlHelper * GlfwHelper::getGlHelper() {
	return openglHelper;
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
			glfwHelper->getGlHelper()->move(GlHelper::UP);
			break;
		case GLFW_KEY_DOWN:
			glfwHelper->getGlHelper()->move(GlHelper::DOWN);
			break;
		case GLFW_KEY_LEFT:
			glfwHelper->getGlHelper()->move(GlHelper::LEFT);
			break;
		case GLFW_KEY_RIGHT:
			glfwHelper->getGlHelper()->move(GlHelper::RIGHT);
			break;
		}
	}

}

GlfwHelper::GlfwHelper() {
	if (!glfwInit()) {
		std::cout << "init problem" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle",
			NULL,
			NULL);
	if (window == NULL) {
		fprintf( stderr,
				"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return;
	}

	glfwSetWindowUserPointer(window, this);

	glfwMakeContextCurrent(window);

// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}

// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, key_callback);

	openglHelper = new GlHelper();
	if (openglHelper == NULL) {
		glfwTerminate();
	}

}
