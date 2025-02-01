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
#include "MappedPBRMaterial.h"
#include "AlternativeObjLoader.h"
#include "NonIndexedOBJ.h"
#include "Transformation.h"
#include "DirectionalLight.h"
#include "Framebuffer.h"
#include "DepthBuffer.h"

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

glm::vec2 lastMousePos(0.0f, 0.0f);
bool firstMouse = true;
bool isLeftMouseDown = false;

PerspectiveCamera* cameraPtr = nullptr;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isLeftMouseDown = true;
        } else if (action == GLFW_RELEASE) {
            isLeftMouseDown = false;
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    glm::vec2 currentPos(xPos, yPos);
    glm::vec2 delta = currentPos - lastMousePos;
    lastMousePos = currentPos;

    if (firstMouse) {
        delta = glm::vec2(0.0f);
        firstMouse = false;
    }

    if (isLeftMouseDown) {
        if(cameraPtr!=nullptr) {
            cameraPtr->transform.rotateAroundPivot(vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), -delta.x/100.f);
            glm::vec3 right = glm::normalize(glm::cross(cameraPtr->wLookat - cameraPtr->wEye(), cameraPtr->wVup));
            cameraPtr->transform.rotateAroundPivot(vec3(0.0f), right, -delta.y/100.f);
        }
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
   if(cameraPtr != nullptr) {
        if(yoffset > 0) {
            cameraPtr->transform.position *= 1.0f/1.1f;
        } else {
            cameraPtr->transform.position *= 1.1f;
        }
    }
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

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
    auto pbr_frag_shader_mapped = std::make_shared<Shader>();
    auto light_frag_shader = std::make_shared<Shader>();
    auto screen_quad_vertex = std::make_shared<Shader>();
    auto screen_quad_fragment = std::make_shared<Shader>();
    auto depth_map_vertex = std::make_shared<Shader>();
    auto depth_map_fragment = std::make_shared<Shader>();

    try {
        vertex_shader->load(PathResolver::resolvePath("shaders/vertex_shader.glsl"));
        fragment_shader->load(PathResolver::resolvePath("shaders/fragment_shader.glsl"));
        texture_frag_shader->load(PathResolver::resolvePath("shaders/texture_frag_shader.glsl"));
        normal_geo_shader->load(PathResolver::resolvePath("shaders/normals_geometry_shader.glsl"));
        normal_frag_shader->load(PathResolver::resolvePath("shaders/normals_fragment_shader.glsl"));
        pbr_frag_shader->load(PathResolver::resolvePath("shaders/pbr_fragment.glsl"));
        pbr_frag_shader_mapped->load(PathResolver::resolvePath("shaders/pbr_fragment_mapped.glsl"));

        light_frag_shader->load(PathResolver::resolvePath("shaders/light_fragment.glsl"));

        screen_quad_vertex->load(PathResolver::resolvePath("shaders/screen_quad_vertex.glsl"));
        screen_quad_fragment->load(PathResolver::resolvePath("shaders/screen_quad_fragment.glsl"));

        depth_map_vertex->load(PathResolver::resolvePath("shaders/depth_map.vertex"));
        depth_map_fragment->load(PathResolver::resolvePath("shaders/depth_map.fragment"));

        vertex_shader->compile(GL_VERTEX_SHADER);
        fragment_shader->compile(GL_FRAGMENT_SHADER);
        texture_frag_shader->compile(GL_FRAGMENT_SHADER);
        normal_geo_shader->compile(GL_GEOMETRY_SHADER);
        normal_frag_shader->compile(GL_FRAGMENT_SHADER);
        pbr_frag_shader->compile(GL_FRAGMENT_SHADER);
        pbr_frag_shader_mapped->compile(GL_FRAGMENT_SHADER);

        light_frag_shader->compile(GL_FRAGMENT_SHADER);

        screen_quad_vertex->compile(GL_VERTEX_SHADER);
        screen_quad_fragment->compile(GL_FRAGMENT_SHADER);

        depth_map_vertex->compile(GL_VERTEX_SHADER);
        depth_map_fragment->compile(GL_FRAGMENT_SHADER);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    auto program = std::make_shared<Program>();
    auto texturedProgram = std::make_shared<Program>();
    auto normalProgram = std::make_shared<Program>();
    auto pbrProgram = std::make_shared<Program>();
    auto mappedPbrProgram = std::make_shared<Program>();
    auto lightProgram = std::make_shared<Program>();
    auto screenQuadProgram = std::make_shared<Program>();
    auto depthMapProgram = std::make_shared<Program>();

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

        mappedPbrProgram->attachShader(vertex_shader);
        mappedPbrProgram->attachShader(pbr_frag_shader_mapped);
        mappedPbrProgram->link();

        lightProgram->attachShader(vertex_shader);
        lightProgram->attachShader(light_frag_shader);
        lightProgram->link();

        screenQuadProgram->attachShader(screen_quad_vertex);
        screenQuadProgram->attachShader(screen_quad_fragment);
        screenQuadProgram->link();

        depthMapProgram->attachShader(depth_map_vertex);
        depthMapProgram->attachShader(depth_map_fragment);
        depthMapProgram->link();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    ImageTexture2D uvgrid(PathResolver::resolvePath("textures/uvgrid.png"));
    auto cerberusAlbedo = std::make_shared<ImageTexture2D>(PathResolver::resolvePath("textures/cerberus_albedo.png"));
    auto cerberusMetallic = std::make_shared<ImageTexture2D>(PathResolver::resolvePath("textures/cerberus_metalness.png"), ImageTexture2D::Type::Data);
    auto cerberusRoughness = std::make_shared<ImageTexture2D>(PathResolver::resolvePath("textures/cerberus_roughness.png"), ImageTexture2D::Type::Data);
    auto cerberusNormal = std::make_shared<ImageTexture2D>(PathResolver::resolvePath("textures/cerberus_normal.png"), ImageTexture2D::Type::Normal);
    auto brickNormal = std::make_shared<ImageTexture2D>(PathResolver::resolvePath("textures/brick_normal.png"), ImageTexture2D::Type::Normal);
    auto fullWhiteTexture = std::make_shared<SingleValueTexture>(vec4(1.0f, 1.0f, 1.0f, 1.0f));

    try{
        uvgrid.load();
        cerberusAlbedo->load();
        cerberusMetallic->load();
        cerberusRoughness->load();
        cerberusNormal->load();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    MappedPBRMaterial cerberusMaterial;

    cerberusMaterial.albedo = cerberusAlbedo;
    cerberusMaterial.metallic = cerberusMetallic;
    cerberusMaterial.roughness = cerberusRoughness;
    cerberusMaterial.ao = fullWhiteTexture;
    cerberusMaterial.normal = cerberusNormal;

    auto quadGeometry = std::make_shared<QuadGeometry>();
    auto cubeGeometry = std::make_shared<CubeGeometry>();
    auto monkeyGeometry = std::make_shared<OBJGeometry>(PathResolver::resolvePath("models/monkey.obj"));
    auto smoothIcoSphereGeometry = std::make_shared<OBJGeometry>(PathResolver::resolvePath("models/icosphere_smooth_high.obj"));
    auto flatIcoSphereGeometry = std::make_shared<OBJGeometry>(PathResolver::resolvePath("models/icosphere_flat_high.obj"));
    auto cerberusGeometry = std::make_shared<NonIndexedObj>(PathResolver::resolvePath("models/cerberus.obj"));

    auto quadMesh = std::make_shared<Mesh>(quadGeometry, texturedProgram);
    auto cubeMesh = std::make_shared<Mesh>(cubeGeometry, program, depthMapProgram);
    auto monkeyMesh = std::make_shared<Mesh>(monkeyGeometry, texturedProgram, depthMapProgram);
    auto normalsMesh = std::make_shared<Mesh>(monkeyGeometry, normalProgram);
    auto cerberusMesh = std::make_shared<Mesh>(cerberusGeometry, mappedPbrProgram, depthMapProgram);
    auto cerberusNormalsMesh = std::make_shared<Mesh>(cerberusGeometry, normalProgram);
    auto lightMesh = std::make_shared<Mesh>(cubeGeometry, lightProgram);

    auto smoothIcoSphereMesh = std::make_shared<Mesh>(smoothIcoSphereGeometry, pbrProgram, depthMapProgram);
    auto smoothNormalsMesh = std::make_shared<Mesh>(smoothIcoSphereGeometry, normalProgram);
    auto flatIcoSphereMesh = std::make_shared<Mesh>(flatIcoSphereGeometry, pbrProgram, depthMapProgram);
    auto flatNormalsMesh = std::make_shared<Mesh>(flatIcoSphereGeometry, normalProgram);

    GameObject quad(quadMesh);
    GameObject cube(cubeMesh);
    GameObject monkey(monkeyMesh);
    GameObject normalsObject(normalsMesh);

    GameObject smoothIcoSphere(smoothIcoSphereMesh);
    GameObject flatIcoSphere(flatIcoSphereMesh);
    GameObject smoothNormalsObject(smoothNormalsMesh);
    GameObject flatNormalsObject(flatNormalsMesh);

    GameObject cerberus(cerberusMesh);
    GameObject cerberusNormals(cerberusNormalsMesh);
    cerberus.transform.Scale(vec3(5.0f));

    PointLight light1(vec3(1.0f, 1.0f, 1.0f), vec3(10.0f, 10.0f, 10.0f));
    GameObject light1Object(lightMesh);
    light1Object.transform.Scale(vec3(0.2f));

    PointLight light2(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 50.0f));
    GameObject light2Object(lightMesh);
    light2Object.transform.Scale(vec3(0.2f));

    DirectionalLight sunLight(vec3(-1.0f), vec3(20.0f, 20.0f, 20.0f));

    PBRMaterial material;
    material.albedo = vec3(1.0f, 0.0f, 0.0f);
    material.metallic = 0.7f;
    material.roughness = 0.2f;
    material.ao = 1.0f;

    auto displayInfo = std::make_shared<DisplayInfo>(640, 480, true, DisplayInfo::Windowed, 0);

    PerspectiveCamera camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.01f, 100.0f, displayInfo);
    camera.transform.translate(vec3(0.0f, 0.0f, 3.0f));
    cameraPtr = &camera;

    float time = (float)glfwGetTime();
    float lastTime = time;
    float deltaTime = 0.0f;
    float frameTimerTime = time;
    float fpsDisplayTimer = 0.0f;
    int avgFrameCounter = 0;
    float avgFrameTime = 0.0f;
    float maxFrameTime = 0.0f;

    Framebuffer renderBuffer(640, 480);
    auto fullScreenQuad = std::make_shared<Mesh>(quadGeometry, screenQuadProgram);

    Depthbuffer depthBuffer(1024, 1024);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    displayInfo->width = width;
    displayInfo->height = height;
    glViewport(0, 0, width, height);


    while (!glfwWindowShouldClose(window))
    {
        deltaTime = time - lastTime;
        lastTime = time;
        time = (float)glfwGetTime();

        depthBuffer.bind();
        glViewport(0,0,1024,1024);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        glm::mat4 lightMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 5.0f) * glm::lookAt(-sunLight.direction, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        depthMapProgram->setUniform("lightSpaceMatrix", lightMatrix);
        cerberus.depthMapDraw();
        flatIcoSphere.depthMapDraw();

        renderBuffer.bind();
        glViewport(0,0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mappedPbrProgram->setUniform("lightSpaceMatrix", lightMatrix);
        mappedPbrProgram->setUniform("depthMap", depthBuffer.getTargetTexture(), 5);


        smoothNormalsObject.transform = smoothIcoSphere.transform;
        //smoothIcoSphere.draw(camera, light1, light2, sunLight, material);
        //smoothNormalsObject.draw(camera);

        flatNormalsObject.transform = flatIcoSphere.transform;
        //flatIcoSphere.draw(camera, light1, light2, sunLight, material);
        //flatNormalsObject.draw(camera);

        cerberusNormals.transform = cerberus.transform;
        cerberus.draw(camera, cerberusMaterial, sunLight);
        //cerberusNormals.draw(camera);

        light1.position = vec3(2.5f * cos(time/2.0f), 0.85f, 2.5f * sin(time/2.0f));
        light1Object.transform.position = light1.position;
        lightProgram->setUniform("color", light1.color);
        //light1Object.draw(camera);

        light2.position = vec3(2.5f * cos(time), 0.85f, 2.5f * sin(time));
        light2Object.transform.position = light2.position;
        lightProgram->setUniform("color", light2.color);
        //light2Object.draw(camera);

        
        Framebuffer::bindDefault();
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        fullScreenQuad->getProgram()->setUniform("image", renderBuffer.getTargetTexture(), 0);
        //fullScreenQuad->getProgram()->setUniform("depthMap", depthBuffer.getTargetTexture(), 0);
        fullScreenQuad->draw();


        glfwSwapBuffers(window);
        glfwPollEvents();
        float frametime = (float)glfwGetTime() - frameTimerTime;
        float FPS = 1.0f / frametime;
        avgFrameTime += frametime;
        avgFrameCounter++;
        if(frametime > maxFrameTime) {
            maxFrameTime = frametime;
        }
        if(fpsDisplayTimer > 1.0f) {
            float avgFrametime = avgFrameTime / avgFrameCounter;
            float avgFPS = 1.0f / avgFrametime;
            float minFPS = 1.0f / maxFrameTime;
            //std::cout << "Average Frame Time:       " << avgFrametime * 1000.0f << " ms, Average FPS:       " << avgFPS << std::endl;
            //std::cout << "Max Frame Time:       " << maxFrameTime * 1000.0f << " ms, Min FPS:       " << minFPS << std::endl;
            avgFrameTime = 0.0f;
            avgFrameCounter = 0;
            fpsDisplayTimer = 0.0f;
            maxFrameTime = 0.0f;
        }
        fpsDisplayTimer += frametime;
        frameTimerTime = (float)glfwGetTime();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}