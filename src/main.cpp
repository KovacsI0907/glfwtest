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
#include "ImageTexture2D.h"
#include "QuadGeometry.h"
#include "PointLight.h"
#include "PBRMaterial.h"
#include "SingleValueTexture.h"

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
    auto texture_frag_shader = std::make_shared<Shader>();
    auto normal_geo_shader = std::make_shared<Shader>();
    auto normal_frag_shader = std::make_shared<Shader>();
    auto pbr_frag_shader = std::make_shared<Shader>();
    try {
        vertex_shader->load(PathResolver::resolvePath("shaders/vertex_shader.glsl"));
        fragment_shader->load(PathResolver::resolvePath("shaders/fragment_shader.glsl"));
        texture_frag_shader->load(PathResolver::resolvePath("shaders/texture_frag_shader.glsl"));
        normal_geo_shader->load(PathResolver::resolvePath("shaders/normals_geometry_shader.glsl"));
        normal_frag_shader->load(PathResolver::resolvePath("shaders/normals_fragment_shader.glsl"));
        pbr_frag_shader->load(PathResolver::resolvePath("shaders/pbr_fragment.glsl"));

        vertex_shader->compile(GL_VERTEX_SHADER);
        fragment_shader->compile(GL_FRAGMENT_SHADER);
        texture_frag_shader->compile(GL_FRAGMENT_SHADER);
        normal_geo_shader->compile(GL_GEOMETRY_SHADER);
        normal_frag_shader->compile(GL_FRAGMENT_SHADER);
        pbr_frag_shader->compile(GL_FRAGMENT_SHADER);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    auto program = std::make_shared<Program>();
    auto texturedProgram = std::make_shared<Program>();
    auto normalProgram = std::make_shared<Program>();
    auto pbrProgram = std::make_shared<Program>();

    try{
        program->attachShader(vertex_shader);
        program->attachShader(fragment_shader);
        program->link();

        texturedProgram->attachShader(vertex_shader);
        texturedProgram->attachShader(texture_frag_shader);
        texturedProgram->link();

        normalProgram->attachShader(vertex_shader);
        normalProgram->attachShader(normal_frag_shader);
        normalProgram->attachShader(normal_geo_shader);
        normalProgram->link();

        pbrProgram->attachShader(vertex_shader);
        pbrProgram->attachShader(pbr_frag_shader);
        pbrProgram->link();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    ImageTexture2D uvgrid(PathResolver::resolvePath("textures/uvgrid.png"));
    SingleValueTexture fullWhiteTexture(vec4(1.0f, 1.0f, 1.0f, 1.0f));

    try{
        uvgrid.load();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    auto quadGeometry = std::make_shared<QuadGeometry>();
    auto cubeGeometry = std::make_shared<CubeGeometry>();
    auto monkeyGeometry = std::make_shared<OBJGeometry>(PathResolver::resolvePath("models/monkey.obj"));
    auto smoothIcoSphereGeometry = std::make_shared<OBJGeometry>(PathResolver::resolvePath("models/icosphere_smooth_high.obj"));
    auto flatIcoSphereGeometry = std::make_shared<OBJGeometry>(PathResolver::resolvePath("models/icosphere_flat_high.obj"));

    auto quadMesh = std::make_shared<Mesh>(quadGeometry, texturedProgram);
    auto cubeMesh = std::make_shared<Mesh>(cubeGeometry, program);
    auto monkeyMesh = std::make_shared<Mesh>(monkeyGeometry, texturedProgram);
    auto normalsMesh = std::make_shared<Mesh>(monkeyGeometry, normalProgram);

    auto smoothIcoSphereMesh = std::make_shared<Mesh>(smoothIcoSphereGeometry, pbrProgram);
    auto smoothNormalsMesh = std::make_shared<Mesh>(smoothIcoSphereGeometry, normalProgram);
    auto flatIcoSphereMesh = std::make_shared<Mesh>(flatIcoSphereGeometry, pbrProgram);
    auto flatNormalsMesh = std::make_shared<Mesh>(flatIcoSphereGeometry, normalProgram);

    GameObject quad(quadMesh);
    GameObject cube(cubeMesh);
    GameObject monkey(monkeyMesh);
    GameObject normalsObject(normalsMesh);

    GameObject smoothIcoSphere(smoothIcoSphereMesh);
    GameObject flatIcoSphere(flatIcoSphereMesh);
    GameObject smoothNormalsObject(smoothNormalsMesh);
    GameObject flatNormalsObject(flatNormalsMesh);

    PointLight light1(vec3(3.0f, 3.0f, 1.0f), vec3(100.0f, 100.0f, 100.0f));
    PBRMaterial material;
    material.albedo = vec3(1.0f, 0.0f, 0.0f);
    material.metallic = 0.2f;
    material.roughness = 0.2f;
    material.ao = 1.0f;



    auto displayInfo = std::make_shared<DisplayInfo>(640, 480, true, DisplayInfo::Windowed, 0);

    PerspectiveCamera camera(vec3(0.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.01f, 100.0f, displayInfo);

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

        texturedProgram->setUniform("albedoMap", fullWhiteTexture, 0);
        quad.draw(camera);

        cube.transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), time);
        program->setUniform("color", vec3(0.0f, 1.0f, 0.0f));
        //cube.draw(camera);

        texturedProgram->use();
        monkey.transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), time);
        normalsObject.transform = monkey.transform;
        //monkey.draw(camera);
        //normalsObject.draw(camera);

        material.uploadUniforms(pbrProgram);
        light1.uploadUniforms(pbrProgram);

        smoothIcoSphere.transform.position = (glm::vec3(-1.0f, 0.0f, 0.0f));
        smoothIcoSphere.transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), time);
        smoothNormalsObject.transform = smoothIcoSphere.transform;
        //smoothIcoSphere.draw(camera);
        //smoothNormalsObject.draw(camera);

        flatIcoSphere.transform.position = (glm::vec3(1.0f, 0.0f, 0.0f));
        flatIcoSphere.transform.rotate(glm::vec3(0.0f, 1.0f, 0.0f), time);
        flatNormalsObject.transform = flatIcoSphere.transform;
        //flatIcoSphere.draw(camera);
        //flatNormalsObject.draw(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}