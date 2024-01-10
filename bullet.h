#pragma once
// Bullet.h
#ifndef BULLET_H
#define BULLET_H
#define PI 3.1415

#include <glm/glm.hpp>
#include <memory>

class Bullet {
public:
    glm::vec3 position;
    glm::vec3 direction;
    float speed;
    float radius;
    bool active;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    const int Y_SEGMENTS = 50;
    const int X_SEGMENTS = 50;

    // Constructor to initialize bullet parameters
    Bullet(glm::vec3 pos, glm::vec3 dir, float spd, float rad)
        : position(pos), direction(glm::normalize(dir)), speed(spd), radius(rad), active(true) {
            // 生成球的顶点数据
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 360, nullptr, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            // 顶点位置属性
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindVertexArray(0);
    }


    void draw() {

        // 更新顶点数据
        std::vector<glm::vec3> vertices;
        std::vector<unsigned int> indices;

        for (int lat = 0; lat <= Y_SEGMENTS; lat++) {
            float theta = lat * PI / Y_SEGMENTS;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= X_SEGMENTS; lon++) {
                float phi = lon * 2 * PI / X_SEGMENTS;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;

                vertices.push_back(glm::vec3(position.x + radius * x, position.y + radius * y, position.z + radius * z));
            }
        }

        for (int lat = 0; lat < Y_SEGMENTS; lat++) {
            for (int lon = 0; lon < X_SEGMENTS; lon++) {
                int first = lat * (X_SEGMENTS + 1) + lon;
                int second = first + 1;
                int third = (lat + 1) * (X_SEGMENTS + 1) + lon;
                int fourth = third + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(third);

                indices.push_back(second);
                indices.push_back(fourth);
                indices.push_back(third);
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

        // 绘制球的线框
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Deactivate the bullet (for instance, when it hits a model)
    void deactivate() {
        active = false;
        speed = 0.0f;
    }

    glm::vec3 getDirection(){
        return direction;
    }

    float getSpeed() {
        return speed;
    }

    float getRadius() {
        return radius;
    }

    glm::vec3 getPosition() {
        return position;
    }

    void setDirection(glm::vec3 dir) {
        direction = dir;
    }
};

#endif // BULLET_H
