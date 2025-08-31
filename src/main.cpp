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

float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,
                    -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, 0.5f,  0.0f,
                    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,
                    -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  -1.0f,
                    0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
                    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,
                    0.5f,  1.0f,  0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
                    -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
                    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
                    0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

std::vector<glm::vec3> cubes;

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float CAM_KEYBOARD_ROT_SPEED = 200.0f;

GLuint VBO, cubeVAO;
std::unique_ptr<Shader> mainShader;
GLuint lightVAO;
std::unique_ptr<Shader> lightcubeShader;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera camera(glm::vec3(-1.0f, 2.0f, 10.0f));
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastMouseX = SCR_WIDTH / 2.0, lastMouseY = SCR_HEIGHT / 2.0;

std::mt19937 gen;

// imgui settings
float ambientStrength = 0.1;
float specularStrength = 0.5;
bool movingLight = true;

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
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

    float time = glfwGetTime();
    if (movingLight)
        lightPos = glm::vec3(sinf(time), sinf(time), cosf(time));
    // be sure to activate shader when setting uniforms/drawing objects
    mainShader->use();
    mainShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    mainShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    mainShader->setVec3("lightPos", lightPos);
    mainShader->setVec3("viewPos", camera.Position);
    mainShader->setFloat("ambientStrength", ambientStrength);
    mainShader->setFloat("specularStrength", specularStrength);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    mainShader->setMat4("projection", projection);
    mainShader->setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    mainShader->setMat4("model", model);

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    for (auto cubepos : cubes) {
        mainShader->setMat4("model", glm::translate(glm::mat4(1.0), cubepos));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object
    lightcubeShader->use();
    lightcubeShader->setMat4("projection", projection);
    lightcubeShader->setMat4("view", view);
    model = glm::mat4(1.0f);
    // model = glm::translate(model, lightPos);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightcubeShader->setMat4("model", model);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void drawImgui() {
    if (ImGui::CollapsingHeader("Settings")) {
        ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 2.0f);
        ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 200.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
        ImGui::Checkbox("Move Light", &movingLight);
    }
}

void processInput(GLFWwindow *window) {
    bool update_rot = false;
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
