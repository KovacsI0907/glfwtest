#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Shader.h"
#include "Program.h"
#include "PathResolver.h"
#include "UniformProvider.h"
#include "ColoredTriangle.h"
#include "Mesh.h"
#include "GameObject.h"

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    auto vertex_shader = std::make_shared<Shader>();
    auto fragment_shader = std::make_shared<Shader>();
    try {
        vertex_shader->load(PathResolver::resolvePath("shaders/vertex_shader.glsl"));
        fragment_shader->load(PathResolver::resolvePath("shaders/fragment_shader.glsl"));

        vertex_shader->compile(GL_VERTEX_SHADER);
        fragment_shader->compile(GL_FRAGMENT_SHADER);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    auto program = std::make_shared<Program>();

    try{
        program->attachShader(vertex_shader);
        program->attachShader(fragment_shader);
        program->link();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    auto coloredTriangleGeometry = std::make_shared<ColoredTriangle>();

    auto coloredTriangleMesh = std::make_shared<Mesh>(coloredTriangleGeometry, program);

    GameObject gameObject(coloredTriangleMesh);

    float time = (float)glfwGetTime();
    float lastTime = time;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glm::mat4 p, vp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        deltaTime = time - lastTime;
        lastTime = time;
        time = (float)glfwGetTime();

        gameObject.transform.rotate(glm::vec3(0.0f, 0.0f, 1.0f), deltaTime);
        p = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        vp = p;

        UniformProvider::setUniform(program->getID(), "VP", vp);
        gameObject.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}