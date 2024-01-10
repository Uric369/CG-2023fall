#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "Shader.h"

// GLFW
#include <GLFW/glfw3.h>


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

float* getColorFromTime(GLfloat time);
float* getPointAttriFromTime(GLfloat time);
float* glitterSize(GLfloat time);
float* glitterColor(GLfloat time);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

bool isChangingColor = false;
bool isGlowing = false;
bool isGlitter = false;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in float colorIndice;\n"
"uniform vec3 colors[10];"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = vec3(colors[int(colorIndice)]);\n"
"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";

const GLchar* lineVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* lineFragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";

const GLchar* pointVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in float index;\n"
"uniform vec2 pointAttri[9];"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"gl_PointSize = pointAttri[int(index)].x;"
"ourColor = vec3(pointAttri[int(index)].y, pointAttri[int(index)].y, pointAttri[int(index)].y);\n"
"}\0";
const GLchar* pointFragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"vec2 center = vec2(0.5, 0.5);"
"vec2 toCenter = gl_PointCoord - center;"
"float distanceToCenter = length(toCenter);"
"if (distanceToCenter > 0.5) { discard; }"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";

const GLchar* glowVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in float index;\n"
"uniform vec2 pointAttri[9];"
"uniform vec3 colors[9];"

"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"gl_PointSize = pointAttri[int(index)].x;"
"ourColor = vec3(colors[int(index)]);\n"
"}\0";
const GLchar* glowFragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"vec2 center = vec2(0.5, 0.5);"
"vec2 toCenter = gl_PointCoord - center;"
"float distanceToCenter = length(toCenter);"
"if (distanceToCenter > 0.5) { discard; }"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";






// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);



    // Set up vertex data (and buffer(s)) and attribute pointers


    GLfloat vertices[] = {
     0.0f,  0.8f, 0.0f,
     -0.6928f, 0.4f, 0.0f,
    -0.6928f,  -0.4f, 0.0f,
     0.0f,  -0.8f, 0.0f,
     0.6928f,  -0.4f, 0.0f,
     0.6928f,  0.4f, 0.0f,

    -0.5f, -0.1f, 0.0f,
    0.5f, 0.2f, 0.0f,
    0.2f, -0.5f, 0.0f,
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 6,  // top left
        1, 2, 6,   // left
        2, 3, 6,  //bottom left
        3, 6, 8,
        3, 4, 8,
        4, 7, 8,
        6, 7, 8,
        0, 6, 7,
        4, 5, 7,
        0, 5, 7,
    };

    float colors[] = {
    0.9648f, 0.58203f, 0.8164f,
    0.8789f, 0.4257f, 0.6953f,
    0.54687f, 0.04267f, 0.3008f,
    0.3984f, 0.015625f, 0.1914f,
    0.7383f, 0.230469f, 0.46484f,
    0.60937f, 0.105468f, 0.3672f,
    0.54687f, 0.04267f, 0.3008f,
    0.7656f, 0.2031f, 0.53515f,
    0.4023f, 0.0234f, 0.1875f,
    0.5469f, 0.05078f, 0.3047f,
    };

    GLuint colorIndices[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    };

    GLfloat pointAttri[] = {
        5, 1,
        10, 1,
        15, 1,
        20, 1,
        15, 1,
        10, 1,
        5, 1,
        10, 1,
        15, 1,
    };

    

    // Build and compile our shader program
// Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex shader
    GLuint lineVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(lineVertexShader, 1, &lineVertexShaderSource, NULL);
    glCompileShader(lineVertexShader);
    // Check for compile time errors
    glGetShaderiv(lineVertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(lineVertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint lineFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(lineFragmentShader, 1, &lineFragmentShaderSource, NULL);
    glCompileShader(lineFragmentShader);
    // Check for compile time errors
    glGetShaderiv(lineFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(lineFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint lineShaderProgram = glCreateProgram();
    glAttachShader(lineShaderProgram, lineVertexShader);
    glAttachShader(lineShaderProgram, lineFragmentShader);
    glLinkProgram(lineShaderProgram);
    // Check for linking errors
    glGetProgramiv(lineShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(lineShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(lineVertexShader);
    glDeleteShader(lineFragmentShader);


    // Vertex shader
    GLuint pointVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(pointVertexShader, 1, &pointVertexShaderSource, NULL);
    glCompileShader(pointVertexShader);
    // Check for compile time errors
    glGetShaderiv(pointVertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(pointVertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint pointFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(pointFragmentShader, 1, &pointFragmentShaderSource, NULL);
    glCompileShader(pointFragmentShader);
    // Check for compile time errors
    glGetShaderiv(pointFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(pointFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint pointShaderProgram = glCreateProgram();
    glAttachShader(pointShaderProgram, pointVertexShader);
    glAttachShader(pointShaderProgram, pointFragmentShader);
    glLinkProgram(pointShaderProgram);
    // Check for linking errors
    glGetProgramiv(pointShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(pointShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(pointVertexShader);
    glDeleteShader(pointFragmentShader);

    // Vertex shader
    GLuint glowVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(glowVertexShader, 1, &glowVertexShaderSource, NULL);
    glCompileShader(glowVertexShader);
    // Check for compile time errors
    glGetShaderiv(glowVertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(glowVertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint glowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(glowFragmentShader, 1, &pointFragmentShaderSource, NULL);
    glCompileShader(glowFragmentShader);
    // Check for compile time errors
    glGetShaderiv(glowFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(glowFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint glowShaderProgram = glCreateProgram();
    glAttachShader(glowShaderProgram, glowVertexShader);
    glAttachShader(glowShaderProgram, glowFragmentShader);
    glLinkProgram(glowShaderProgram);
    // Check for linking errors
    glGetProgramiv(glowShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(glowShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(glowVertexShader);
    glDeleteShader(glowFragmentShader);





    GLuint VBOs[10], VAOs[10];
    glGenVertexArrays(10, VAOs); // We can also generate multiple VAOs or buffers at the same time
    glGenBuffers(10, VBOs);

    for (int i = 0; i < 10; i++) {
        std::cout << "generate:" << i << std::endl;
        glBindVertexArray(VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);

        GLfloat vertex[12];
        // Get the indices for the ith triangle
        GLuint startIndex = 3 * i;

        for (int j = 0; j < 3; j++) {
            GLuint index = indices[startIndex + j];
            vertex[j * 4] = vertices[index * 3]; //x
            vertex[j * 4 + 1] = vertices[index * 3 + 1]; //y
            vertex[j * 4 + 2] = vertices[index * 3 + 2]; //z
        }

        // Extract vertex colors
        for (int j = 0; j < 3; j++){
            vertex[j * 4 + 3] = static_cast<float>(colorIndices[i]);
        }


        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 , vertex, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader (same shader for both triangles)
        glUseProgram(shaderProgram);
;
        GLint location = glGetUniformLocation(shaderProgram, "colors");
        if (location != -1) {
            if (!isChangingColor) glUniform3fv(location, 10, colors);
            else {
                GLfloat timeValue = glfwGetTime();
                glUniform3fv(location, 10, getColorFromTime(timeValue));
            }
        }


        for (int i = 0; i < 10; i++) {
            // Draw first triangle using the data from the first VAO
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        glBindVertexArray(0);
        
        if (isGlowing) {
            glUseProgram(lineShaderProgram);
            for (int i = 0; i < 10; i++) {
                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_LINE_LOOP, 0, 3);
            }


            glUseProgram(pointShaderProgram);
            glEnable(GL_PROGRAM_POINT_SIZE);
            for (int i = 0; i < 9; i++) {
                GLint pointlocation = glGetUniformLocation(shaderProgram, "pointAttri");
                GLfloat timeValue = glfwGetTime();
                glUniform2fv(location, 9, getPointAttriFromTime(timeValue));
                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_POINTS, 0, 3);
            }
            glDisable(GL_PROGRAM_POINT_SIZE);
        }

        if (isGlitter) {
            glUseProgram(lineShaderProgram);
            for (int i = 0; i < 10; i++) {
                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_LINE_LOOP, 0, 3);
            }

            glUseProgram(glowShaderProgram);
            glEnable(GL_PROGRAM_POINT_SIZE);
            for (int i = 0; i < 9; i++) {
                GLint pointlocation = glGetUniformLocation(glowShaderProgram, "pointAttri");
                GLint glowlocation = glGetUniformLocation(glowShaderProgram, "colors");
                GLfloat timeValue = glfwGetTime();
                glUniform2fv(pointlocation, 9, glitterSize(timeValue));
                glUniform3fv(glowlocation, 9, glitterColor(timeValue));

                glBindVertexArray(VAOs[i]);
                glDrawArrays(GL_POINTS, 0, 3);
            }
            glDisable(GL_PROGRAM_POINT_SIZE);
        }


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(10, VAOs);
    glDeleteBuffers(10, VBOs);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 当按下 C 键时，更改 isChangingColor 的值
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        isChangingColor = true;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        isGlowing = true;
    }

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        isChangingColor = false;
        isGlowing = false;
        isGlitter = false;
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        isGlowing = false;
        isGlitter = true;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

float* getColorFromTime(GLfloat time) {
    static float newColor[30];

    // 初始颜色
    float initialColors[] = {
        0.9648f, 0.58203f, 0.8164f,
        0.8789f, 0.4257f, 0.6953f,
        0.54687f, 0.04267f, 0.3008f,
        0.3984f, 0.015625f, 0.1914f,
        0.7383f, 0.230469f, 0.46484f,
        0.60937f, 0.105468f, 0.3672f,
        0.54687f, 0.04267f, 0.3008f,
        0.7656f, 0.2031f, 0.53515f,
        0.4023f, 0.0234f, 0.1875f,
        0.5469f, 0.05078f, 0.3047f
    };
    
    GLfloat seed = (sin(time) / 2) + 0.5;
    float rate = 0.5;

    for (int i = 0; i < 10; i++) {
        newColor[3 * i] = initialColors[3 * i] - seed * rate;
        newColor[3 * i + 1] = initialColors[3 * i + 1] + 0.5 * seed * rate;
        newColor[3 * i + 2] = initialColors[3 * i + 1] +  seed * rate;
    }

    return newColor;
}


float* getPointAttriFromTime(GLfloat time) {
    static float newPointAttri[18];

    // 初始颜色
    GLfloat initPointAttri[] = {
    5, 0.7,
    10, 0.8,
    15, 0.9,
    20, 1,
    15, 0.8,
    10, 0.7,
    5, 0.6,
    10, 0.8,
    15, 0.9,
    };

    GLfloat pointTrend[] = {
        1, 1, -1, -1, -1, 1, 1, 1, 1,
    };

    GLfloat seed = (sin(time) / 2) + 0.5;
    float sizeRate = 17;
    float transRate = 0.4;

    for (int i = 0; i < 9; i++) {
        newPointAttri[2 * i] = initPointAttri[2 * i] + seed * sizeRate * pointTrend[i];
        newPointAttri[2 * i + 1] = initPointAttri[2 * i + 1] - seed * transRate * pointTrend[i];
    }

    return newPointAttri;

}

float* glitterSize(GLfloat time) {
    static float newPointAttri[18];

    // 初始颜色
    GLfloat initPointAttri[] = {
    5, 0.7,
    10, 0.8,
    15, 0.9,
    20, 1,
    15, 0.8,
    10, 0.7,
    5, 0.6,
    10, 0.8,
    15, 0.9,
    };

    GLfloat pointTrend[] = {
        1, 1, -1, -1, -1, 1, 1, 1, 1,
    };

    float frequency = 3.0;
    GLfloat seed = (sin(time * frequency) / 2) + 0.5;
    float sizeRate = 17;

    for (int i = 0; i < 9; i++) {
        newPointAttri[2 * i] = initPointAttri[2 * i] + seed * sizeRate * pointTrend[i];
    }

    return newPointAttri;

}


float* glitterColor(GLfloat time) {
    static float newColor[27];

    // 初始颜色
    float initialColors[] = {
        0.9648f, 0.58203f, 0.8164f,
        0.8789f, 0.4257f, 0.6953f,
        0.54687f, 0.04267f, 0.3008f,
        0.3984f, 0.015625f, 0.1914f,
        0.7383f, 0.230469f, 0.46484f,
        0.60937f, 0.105468f, 0.3672f,
        0.54687f, 0.04267f, 0.3008f,
        0.7656f, 0.2031f, 0.53515f,
        0.4023f, 0.0234f, 0.1875f,
    };

    float frequency = 12.0;
    GLfloat seed = (sin(time * frequency) / 2) + 0.5;
    float rate = 0.8;

    for (int i = 0; i < 9; i++) {
        newColor[3 * i] = initialColors[3 * i] - seed * rate;
        newColor[3 * i + 1] = initialColors[3 * i + 1] + 0.7 * seed * rate;
        newColor[3 * i + 2] = initialColors[3 * i + 1] + 0.6 * seed * rate;
    }

    return newColor;
}