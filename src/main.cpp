#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>
GLFWwindow* window;
int main() {
    if (glfwInit() != GLFW_TRUE) {
        throw std::runtime_error("GLEW failed to init");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(640, 480, "ImguiBase", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW failed to init");
    }
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "OpenGL version supported: " << version << std::endl;
    return 0;
}