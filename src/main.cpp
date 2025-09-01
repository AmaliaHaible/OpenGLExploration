#include <glad/glad.h>

#include "camera.hpp"
#include "shader.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "glm/common.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include <filesystem>
#include <iostream>
#include <math.h>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void callback_framebuffer_size(GLFWwindow *window, int width, int height);
void callback_key(GLFWwindow *window, int key, int scancode, int action, int mods);
void callback_window_move(GLFWwindow *window, int xpos, int ypos);
void callback_mouse(GLFWwindow *window, double xpos, double ypos);
void callback_scroll(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void draw(GLFWwindow *window);
void drawImgui();
void updateCamera();
void initRandom(std::optional<unsigned> seed = std::nullopt);
float randomFloat(float a, float b);
void initCubes(int count);
GLuint loadTexture(const char *path);
void drawImguiMaterialButtons();

// since i autoformat anyway i just dont care
float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
                    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
                    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
                    0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f,
                    0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f,
                    0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,
                    0.0f,  0.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,
                    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
                    -1.0f, 0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,
                    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
                    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

// GLOBALS
std::vector<glm::vec3> cubes;

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float CAM_KEYBOARD_ROT_SPEED = 200.0f;

GLuint VBO, cubeVAO;
std::unique_ptr<Shader> mainShader;
GLuint lightVAO;
std::unique_ptr<Shader> lightcubeShader;

Camera camera(glm::vec3(-1.0f, 2.0f, 10.0f));
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

GLuint diffuseMap, specularMap;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastMouseX = SCR_WIDTH / 2.0f, lastMouseY = SCR_HEIGHT / 2.0f;

std::mt19937 gen;

glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f), glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                             glm::vec3(1.5f, 0.2f, -1.5f),  glm::vec3(-1.3f, 1.0f, -1.5f)};
// positions of the point lights
glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

// imgui settings
bool movingLight = true;
glm::vec3 materialAmbient(1.0f, 0.5f, 0.31f);
glm::vec3 materialDiffuse(1.0f, 0.5f, 0.31f);
glm::vec3 materialSpecular(0.5f, 0.5f, 0.5f);
float materialShininess = 32.0f;

glm::vec3 lightBaseColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
glm::vec3 lightAmbient = lightBaseColor * glm::vec3(0.5);
glm::vec3 lightDiffuse = lightBaseColor * glm::vec3(0.1);
glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);

int main() {
    initRandom(123);
    initCubes(20);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to Create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, callback_framebuffer_size);
    glfwSetKeyCallback(window, callback_key);
    glfwSetScrollCallback(window, callback_scroll);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mainShader = std::make_unique<Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
    lightcubeShader = std::make_unique<Shader>("shaders/lightcubevertex.glsl", "shaders/lightcubefragment.glsl");
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    diffuseMap = loadTexture("assets/container2.png");
    specularMap = loadTexture("assets/container2_specular.png");
    mainShader->use();
    mainShader->setInt("material.diffuse", 0);
    mainShader->setInt("material.specular", 1);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glfwSwapInterval(1);
    glfwSetWindowRefreshCallback(window, draw);
    glfwSetWindowPosCallback(window, callback_window_move);
    mainShader->use();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    while (!glfwWindowShouldClose(window)) {
        glfwWaitEventsTimeout(1.0 / 60.0);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        draw(window);
        // glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void draw(GLFWwindow *window) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow(); // Show demo window! :)
    drawImgui();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    mainShader->use();
    mainShader->setVec3("viewPos", camera.Position);
    mainShader->setFloat("material.shininess", materialShininess);

    mainShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    mainShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    mainShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    mainShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    mainShader->setVec3("pointLights[0].position", pointLightPositions[0]);
    mainShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    mainShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    mainShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    mainShader->setFloat("pointLights[0].constant", 1.0f);
    mainShader->setFloat("pointLights[0].linear", 0.09f);
    mainShader->setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    mainShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    mainShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    mainShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    mainShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    mainShader->setFloat("pointLights[1].constant", 1.0f);
    mainShader->setFloat("pointLights[1].linear", 0.09f);
    mainShader->setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    mainShader->setVec3("pointLights[2].position", pointLightPositions[2]);
    mainShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    mainShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    mainShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    mainShader->setFloat("pointLights[2].constant", 1.0f);
    mainShader->setFloat("pointLights[2].linear", 0.09f);
    mainShader->setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    mainShader->setVec3("pointLights[3].position", pointLightPositions[3]);
    mainShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    mainShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    mainShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    mainShader->setFloat("pointLights[3].constant", 1.0f);
    mainShader->setFloat("pointLights[3].linear", 0.09f);
    mainShader->setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    mainShader->setVec3("spotLight.position", camera.Position);
    mainShader->setVec3("spotLight.direction", camera.Front);
    mainShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    mainShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    mainShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    mainShader->setFloat("spotLight.constant", 1.0f);
    mainShader->setFloat("spotLight.linear", 0.09f);
    mainShader->setFloat("spotLight.quadratic", 0.032f);
    mainShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    mainShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    mainShader->setMat4("projection", projection);
    mainShader->setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    mainShader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        mainShader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object
    lightcubeShader->use();
    lightcubeShader->setMat4("projection", projection);
    lightcubeShader->setMat4("view", view);
    glBindVertexArray(lightVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        lightcubeShader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void drawImgui() {
    if (ImGui::CollapsingHeader("Light")) {
        ImGui::SliderFloat3("Position", &lightPosition.x, -5.0f, 5.0f);
        if (ImGui::ColorEdit3("BaseColor", &lightBaseColor.x)) {
            lightAmbient = lightBaseColor * glm::vec3(0.5);
            lightDiffuse = lightBaseColor * glm::vec3(0.1);
        };
        ImGui::ColorEdit3("Ambient", &lightAmbient.x);
        ImGui::ColorEdit3("Diffuse", &lightDiffuse.x);
        ImGui::ColorEdit3("Specular", &lightSpecular.x);
    }
    if (ImGui::CollapsingHeader("Material")) {
        ImGui::ColorEdit3("Ambient##2", &materialAmbient.x);
        ImGui::ColorEdit3("Diffuse##2", &materialDiffuse.x);
        ImGui::ColorEdit3("Specular##2", &materialSpecular.x);
        ImGui::SliderFloat("Shininess", &materialShininess, 0.0f, 256.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
        drawImguiMaterialButtons();
    }
}

void processInput(GLFWwindow *window) {
    // -----Movement------
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    // -----Rotation------
    float xoffset = 0, yoffset = 0;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        xoffset -= deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        xoffset += deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        yoffset += deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        yoffset -= deltaTime;
    }
    camera.ProcessMouseMovement(xoffset * CAM_KEYBOARD_ROT_SPEED, yoffset * CAM_KEYBOARD_ROT_SPEED);
}

void initRandom(std::optional<unsigned> seed) {
    if (seed.has_value()) {
        gen.seed(seed.value()); // deterministic
    } else {
        std::random_device rd;
        gen.seed(rd()); // nondeterministic
    }
}

float randomFloat(float a, float b) {
    std::uniform_real_distribution<float> dist(a, b);
    return dist(gen);
}

void initCubes(int count) {
    while (count--) {
        cubes.push_back(glm::vec3(randomFloat(-10.0, 10.0), randomFloat(-5.0, 5.0), -randomFloat(2.0, 10.0)));
    }
}

void drawImguiMaterialButtons() {
    // This is a really ugly function (in case you couldnt tell)
    float window_visible_x2 = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec2 button_sz(100, 30);
    if (ImGui::Button("Emerald", button_sz)) {
        materialAmbient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
        materialDiffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
        materialSpecular = glm::vec3(0.633f, 0.727811f, 0.633f);
        materialShininess = 128.0f * 0.6f;
    }
    float last_button_x2 = ImGui::GetItemRectMax().x;
    float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("Jade", button_sz)) {
        materialAmbient = glm::vec3(0.135f, 0.2225f, 0.1575f);
        materialDiffuse = glm::vec3(0.54f, 0.89f, 0.63);
        materialSpecular = glm::vec3(0.316228f, 0.316228f, 0.316228f);
        materialShininess = 128.0f * 0.1f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("obsidian", button_sz)) {
        materialAmbient = glm::vec3(0.05375f, 0.05f, 0.06625f);
        materialDiffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
        materialSpecular = glm::vec3(0.332741f, 0.328634f, 0.346435f);
        materialShininess = 128.0f * 0.3f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("pearl", button_sz)) {
        materialAmbient = glm::vec3(0.25f, 0.20725f, 0.20725f);
        materialDiffuse = glm::vec3(1.0f, 0.829f, 0.829f);
        materialSpecular = glm::vec3(0.296648f, 0.296648f, 0.296648f);
        materialShininess = 128.0f * 0.088f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("ruby", button_sz)) {
        materialAmbient = glm::vec3(0.1745f, 0.01175f, 0.01175f);
        materialDiffuse = glm::vec3(0.61424f, 0.04136f, 0.04136f);
        materialSpecular = glm::vec3(0.727811f, 0.626959f, 0.626959f);
        materialShininess = 128.0f * 0.6f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("turquoise", button_sz)) {
        materialAmbient = glm::vec3(0.1f, 0.18725f, 0.1745f);
        materialDiffuse = glm::vec3(0.396f, 0.74151f, 0.69102f);
        materialSpecular = glm::vec3(0.297254f, 0.30829f, 0.306678f);
        materialShininess = 128.0f * 0.1f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("brass", button_sz)) {
        materialAmbient = glm::vec3(0.329412f, 0.223529f, 0.027451f);
        materialDiffuse = glm::vec3(0.780392f, 0.568627f, 0.113725f);
        materialSpecular = glm::vec3(0.992157f, 0.941176f, 0.807843f);
        materialShininess = 128.0f * 0.21794872f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("bronze", button_sz)) {
        materialAmbient = glm::vec3(0.2125f, 0.1275f, 0.054f);
        materialDiffuse = glm::vec3(0.714f, 0.4284f, 0.18144f);
        materialSpecular = glm::vec3(0.393548f, 0.271906f, 0.166721f);
        materialShininess = 128.0f * 0.2f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("chrome", button_sz)) {
        materialAmbient = glm::vec3(0.25f, 0.25f, 0.25f);
        materialDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
        materialSpecular = glm::vec3(0.774597f, 0.774597f, 0.774597f);
        materialShininess = 128.0f * 0.6f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("copper", button_sz)) {
        materialAmbient = glm::vec3(0.19125f, 0.0735f, 0.0225f);
        materialDiffuse = glm::vec3(0.7038f, 0.27048f, 0.0828f);
        materialSpecular = glm::vec3(0.256777f, 0.137622f, 0.086014f);
        materialShininess = 128.0f * 0.1f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("gold", button_sz)) {
        materialAmbient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
        materialDiffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
        materialSpecular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
        materialShininess = 128.0f * 0.4f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("silver", button_sz)) {
        materialAmbient = glm::vec3(0.19225f, 0.19225f, 0.19225f);
        materialDiffuse = glm::vec3(0.50754f, 0.50754f, 0.50754f);
        materialSpecular = glm::vec3(0.508273f, 0.508273f, 0.508273f);
        materialShininess = 128.0f * 0.4f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("black plastic", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
        materialDiffuse = glm::vec3(0.01f, 0.01f, 0.01f);
        materialSpecular = glm::vec3(0.50f, 0.50f, 0.50f);
        materialShininess = 128.0f * .25f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("cyan plastic", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.1f, 0.06f);
        materialDiffuse = glm::vec3(0.0f, 0.50980392f, 0.50980392f);
        materialSpecular = glm::vec3(0.50196078f, 0.50196078f, 0.50196078f);
        materialShininess = 128.0f * .25f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("green plastic", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
        materialDiffuse = glm::vec3(0.1f, 0.35f, 0.1f);
        materialSpecular = glm::vec3(0.45f, 0.55f, 0.45f);
        materialShininess = 128.0f * .25f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("red plastic", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
        materialDiffuse = glm::vec3(0.5f, 0.0f, 0.0f);
        materialSpecular = glm::vec3(0.7f, 0.6f, 0.6f);
        materialShininess = 128.0f * .25f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("white plastic", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
        materialDiffuse = glm::vec3(0.55f, 0.55f, 0.55f);
        materialSpecular = glm::vec3(0.70f, 0.70f, 0.70f);
        materialShininess = 128.0f * .25f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("yellow plastic", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
        materialDiffuse = glm::vec3(0.5f, 0.5f, 0.0f);
        materialSpecular = glm::vec3(0.60f, 0.60f, 0.50f);
        materialShininess = 128.0f * .25f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("black rubber", button_sz)) {
        materialAmbient = glm::vec3(0.02f, 0.02f, 0.02f);
        materialDiffuse = glm::vec3(0.01f, 0.01f, 0.01f);
        materialSpecular = glm::vec3(0.4f, 0.4f, 0.4f);
        materialShininess = 128.0f * .078125f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("cyan rubber", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.05f, 0.05f);
        materialDiffuse = glm::vec3(0.4f, 0.5f, 0.5f);
        materialSpecular = glm::vec3(0.04f, 0.7f, 0.7f);
        materialShininess = 128.0f * .078125f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("green rubber", button_sz)) {
        materialAmbient = glm::vec3(0.0f, 0.05f, 0.0f);
        materialDiffuse = glm::vec3(0.4f, 0.5f, 0.4f);
        materialSpecular = glm::vec3(0.04f, 0.7f, 0.04f);
        materialShininess = 128.0f * .078125f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("red rubber", button_sz)) {
        materialAmbient = glm::vec3(0.05f, 0.0f, 0.0f);
        materialDiffuse = glm::vec3(0.5f, 0.4f, 0.4f);
        materialSpecular = glm::vec3(0.7f, 0.04f, 0.04f);
        materialShininess = 128.0f * .078125f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("white rubber", button_sz)) {
        materialAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
        materialDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
        materialSpecular = glm::vec3(0.7f, 0.7f, 0.7f);
        materialShininess = 128.0f * .078125f;
    }
    last_button_x2 = ImGui::GetItemRectMax().x;
    next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
    if (next_button_x2 < window_visible_x2)
        ImGui::SameLine();

    if (ImGui::Button("yellow rubber", button_sz)) {
        materialAmbient = glm::vec3(0.05f, 0.05f, 0.0f);
        materialDiffuse = glm::vec3(0.5f, 0.5f, 0.4f);
        materialSpecular = glm::vec3(0.7f, 0.7f, 0.04f);
        materialShininess = 128.0f * .078125f;
    }
}

void callback_scroll(GLFWwindow *window, double xoffset, double yoffset) { camera.ProcessMouseScroll(static_cast<float>(yoffset)); }

void callback_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
    static bool using_wireframe = 0;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    } else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (using_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            using_wireframe = false;
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            using_wireframe = true;
        }
    }
}

void callback_window_move(GLFWwindow *window, int xpos, int ypos) { draw(window); }

void callback_framebuffer_size(GLFWwindow *window, int width, int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    // std::cout << __FILE_NAME__ << ":" << __LINE__ << ":Resize " << window << " " << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}

GLuint loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
