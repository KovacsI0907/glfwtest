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
#include "Mesh.h"
#include "GameObject.h"
#include "PerspectiveCamera.h"
#include "TriangleGeometry.h"
#include "OpenGLErrorLogger.h"
#include "CubeGeometry.h"
#include "OBJGeometry.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

using namespace glm;

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

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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

    OpenGLErrorLogger::init();

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

    auto coloredTriangleGeometry = std::make_shared<TriangleGeometry>();
    auto coloredTriangleMesh = std::make_shared<Mesh>(coloredTriangleGeometry, program);
    GameObject gameObject(coloredTriangleMesh);

    auto cubeGeometry = std::make_shared<CubeGeometry>();
    auto cubeMesh = std::make_shared<Mesh>(cubeGeometry, program);
    GameObject cube(cubeMesh);

    auto monkeyGeometry = std::make_shared<OBJGeometry>(PathResolver::resolvePath("models/monkey.obj"));
    auto monkeyMesh = std::make_shared<Mesh>(monkeyGeometry, program);
    GameObject monkey(monkeyMesh);


    auto displayInfo = std::make_shared<DisplayInfo>(640, 480, true, DisplayInfo::Windowed, 0);

    PerspectiveCamera camera(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.01f, 100.0f, displayInfo);

    float time = (float)glfwGetTime();
    float lastTime = time;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        displayInfo->width = width;
        displayInfo->height = height;

        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

        deltaTime = time - lastTime;
        lastTime = time;
        time = (float)glfwGetTime();

        gameObject.transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0);
        program->use();
        UniformProvider::setUniform(program->getID(), "color", vec3(1.0f, 0.0f, 0.0f));
        gameObject.draw(camera);

        program->use();
        cube.transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), time);
        UniformProvider::setUniform(program->getID(), "color", vec3(0.0f, 1.0f, 0.0f));
        //cube.draw(camera);

        program->use();
        monkey.transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), time);
        UniformProvider::setUniform(program->getID(), "color", vec3(0.0f, 0.0f, 1.0f));
        monkey.draw(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}