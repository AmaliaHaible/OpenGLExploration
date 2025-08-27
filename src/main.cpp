#include <glad/glad.h>

#include "shader.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include <memory>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void window_move_callback(GLFWwindow *window, int xpos, int ypos);
void processInput(GLFWwindow *window);
void draw(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// float trig_vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
float trig_vertices[] = {
    // positions         // colors
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
};
unsigned int indices[] = {0, 1, 2};
GLuint VBO, VAO, EBO;
std::unique_ptr<Shader> mainShader; 
GLuint vertexShader, fragmentShader;
int main() {
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    mainShader = std::make_unique<Shader>("vertex.glsl", "fragment.glsl");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trig_vertices), trig_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glfwSwapInterval(1);
    glfwSetWindowRefreshCallback(window, draw);
    glfwSetWindowPosCallback(window, window_move_callback);

    while (!glfwWindowShouldClose(window)) {

        processInput(window);
        draw(window);
        // glfwPollEvents();
        glfwWaitEventsTimeout(1.0 / 60.0);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    // static bool using_wireframe = 0;
    // if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    //     glfwSetWindowShouldClose(window, true);
    // } else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
    //     if (using_wireframe) {
    //         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //         using_wireframe = false;
    //     } else {
    //         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //         using_wireframe = true;
    //     }
    // }
}

void draw(GLFWwindow *window) {
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    float timeValue = glfwGetTime();
    float greenValue = 2*((sin(timeValue) / 2.0f) + 0.5f);
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    // glUseProgram(shaderProgram);
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    // glUseProgram(shaderProgram);
    mainShader->setFloat("scale", greenValue);
    mainShader->use();
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
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

void window_move_callback(GLFWwindow *window, int xpos, int ypos) { draw(window); }

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // std::cout << __FILE_NAME__ << ":" << __LINE__ << ":Resize " << window << " " << width << " " << height << std::endl;
    glViewport(0, 0, width, height);
}
