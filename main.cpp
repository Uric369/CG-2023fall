#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "bullet.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void collision_detection(Bullet& bullet, Model& model, float cur_time);
bool testSphereTriangle(const glm::vec3& center, float radius, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
glm::vec3 reflectVec3(glm::vec3 A, glm::vec3 B);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 mesh_normal;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool is_collide = false;
float collision_time;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// lighting
glm::vec3 lightPos(0.8f, 0.8f, 0.8f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader bulletShader("bullet_shader.vs", "bullet_shader.fs");
    Shader shader_outer("9.2.geometry_shader.vs", "9.2.geometry_shader.fs", "9.2.geometry_shader.gs");
    Shader shader_inner("9.2.geometry_shader.vs", "inner_shader.fs", "inner_geo_shader.gs");
    Shader shader_sider("9.2.geometry_shader.vs", "sider_geometry_shader.fs", "sider_geometry_shader.gs");


    // load models
    // -----------
    // "F:/Computer Graphics/bunny_10k.obj"
    // Model nanosuit("./bunny.obj");
    Model nanosuit("F:/Computer Graphics/bunny_10k.obj");
    nanosuit.saveModel("./bunny_save.obj");
    glm::vec3 ini_pos = glm::vec3(18.0f, 35.0f, 10.0f);
    Bullet bullet(ini_pos, -ini_pos, 10.0, 0.1f);
    glm::vec3 direction = bullet.getDirection();
    float speed = bullet.getSpeed();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        if (!is_collide) collision_detection(bullet, nanosuit, currentFrame);

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();;
        glm::mat4 model = glm::mat4(1.0f);
        float time = static_cast<float>(glfwGetTime());
        shader_outer.use();
        shader_outer.setVec3("lightColor", lightColor);
        shader_outer.setVec3("lightPos", lightPos);
        shader_outer.setVec3("viewPos", camera.Position);

        shader_outer.setMat4("projection", projection);
        shader_outer.setMat4("view", view);
        shader_outer.setMat4("model", model);

        // add time component to geometry shader in the form of a uniform
        shader_outer.setFloat("time", time);
        shader_outer.setFloat("collision_time", collision_time);
        shader_outer.setBool("is_collide", is_collide);

        // draw model
        nanosuit.Draw(shader_outer);

        shader_inner.use();
        shader_inner.setVec3("lightColor", lightColor);
        shader_inner.setVec3("lightPos", lightPos);
        shader_inner.setVec3("viewPos", camera.Position);
        shader_inner.setMat4("projection", projection);
        shader_inner.setMat4("view", view);
        shader_inner.setMat4("model", model);

        // add time component to geometry shader in the form of a uniform
        shader_inner.setFloat("time", time);
        shader_inner.setFloat("collision_time", collision_time);
        shader_inner.setBool("is_collide", is_collide);

        nanosuit.Draw(shader_inner);

        shader_sider.use();
        shader_sider.setVec3("lightColor", lightColor);
        shader_sider.setVec3("lightPos", lightPos);
        shader_sider.setVec3("viewPos", camera.Position);
        shader_sider.setMat4("projection", projection);
        shader_sider.setMat4("view", view);
        shader_sider.setMat4("model", model);

        // add time component to geometry shader in the form of a uniform
        shader_sider.setFloat("time", time);
        shader_sider.setFloat("collision_time", collision_time);
        shader_sider.setBool("is_collide", is_collide);
        nanosuit.Draw(shader_sider);

        // In your rendering loop or bullet draw method
        bulletShader.use();
        bulletShader.setVec3("lightColor", lightColor);
        bulletShader.setVec3("lightPos", lightPos);
        bulletShader.setVec3("viewPos", camera.Position);
        bulletShader.setVec3("ini_pos", ini_pos);
        bulletShader.setMat4("model", model); // Replace with your actual model matrix
        bulletShader.setMat4("view", view); // Replace with your actual view matrix
        bulletShader.setMat4("projection", projection); // Replace with your actual projection matrix
        bulletShader.setVec3("direction", direction); // Set the bullet color to green
        bulletShader.setVec3("new_direction", bullet.getDirection()); // Set the bullet color to green
        // add time component to geometry shader in the form of a uniform
        bulletShader.setBool("is_collide", is_collide);
        bulletShader.setFloat("collision_time", collision_time);
        bulletShader.setFloat("time", time);
        bulletShader.setFloat("speed", speed);
        bullet.draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void collision_detection(Bullet& bullet, Model& model, float cur_time) {
    // 获取子弹（球体）的半径和中心位置
    float radius = bullet.getRadius();
    glm::vec3 bullet_center = bullet.getPosition() + bullet.getSpeed() * bullet.getDirection() * cur_time;
    bullet_center.y -= 0.02 * cur_time * cur_time;

    // 遍历模型中的每个网格(mesh)
    for (Mesh& mesh : model.meshes) {
        // 遍历每个网格的三角面片
        for (unsigned int i = 0; i < mesh.indices.size(); i += 3) {
            // 获取三角面片的顶点
            Vertex& v1 = mesh.vertices[mesh.indices[i]];
            Vertex& v2 = mesh.vertices[mesh.indices[i + 1]];
            Vertex& v3 = mesh.vertices[mesh.indices[i + 2]];

            // 检查子弹（球体）是否与三角面片相交
            if (testSphereTriangle(bullet_center, radius, v1.Position, v2.Position, v3.Position)) {
                is_collide = true;
                collision_time = static_cast<float>(glfwGetTime());
                bullet.setDirection(reflectVec3(bullet.getDirection(), mesh_normal));
                std::cout << "collison!!!" << std::endl;
                return; // 发生碰撞，退出检测
            }
        }
    }

    // 如果没有发生碰撞
    is_collide = false;
}

// 检查球体和三角面片是否相交的函数
bool testSphereTriangle(const glm::vec3& center, float radius, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    // 你需要实现这个函数，检测球体和三角面片是否相交
    if (glm::distance(center, v1) > 2 * radius && glm::distance(center, v2) > 2 * radius && glm::distance(center, v3) > 2 * radius) {
        return false;
    }
    else {
        mesh_normal = normalize(cross((v1 - v3), (v2 - v3)));
        float distance = dot((center - v1), mesh_normal);
        if (distance < radius) {
            glm::vec3 collision_point = center - distance * mesh_normal;
            std::cout << "碰撞点：" << collision_point.x << " " << collision_point.y << " " << collision_point.z << std::endl;
            return true;
        }
    }
    return false;
}

glm::vec3 reflectVec3(glm::vec3 A, glm::vec3 B) {
    glm::vec3 B_normalized = normalize(B); // 确保B是单位向量
    float dotAB = dot(A, B_normalized);
    glm::vec3 proj_AB = dotAB * B_normalized;
    glm::vec3 C = (float)2.0 * proj_AB - A;
    return C;
}
