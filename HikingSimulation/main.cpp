#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexShaderManager.h"
#include "FragmentShaderManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// lookat, perspective
// terrain class, shader manager

// Make shader manager for vertex shader and fragment shader
// We need terrain manager as well

// VAO - vertex array object
// VAO Acts as a 'wrapper around' all of the vertex buffer objects
// Important that we glBindVertexArray, this means select the VAO we want to use

// VBO - vertex buffer object

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shader Managers
VertexShaderManager vertexShaderManager;
FragmentShaderManager fragmentShaderManager;

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 in_position;
    void main() {
        gl_Position = vec4(in_position, 1.0);
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 frag_color;
    void main() {
        frag_color = vec4(1.0, 1.0, 0.0, 1.0); // Yellow color
    }
)";


int main()
{
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Start code", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    // Shaders
    // Add error handling, need to know if the shader fails to initialize
    vertexShaderManager.initialize("vertexShader.glsl");
    fragmentShaderManager.initialize("fragmentShader.glsl");

    // Place it in it's own class,
    // read file in class
    // Compile and link shaders
    //GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    //glCompileShader(vertexShader);

    //GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    //glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderManager.getShaderId());
    glAttachShader(shaderProgram, fragmentShaderManager.getShaderId());
    glLinkProgram(shaderProgram);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left vertex position
         0.5f, -0.5f, 0.0f, // right vertex position
         0.0f,  0.5f, 0.0f  // top vertex position
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    // Bind means that we are 'selecting the active buffer object' that we want to work with
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // simulation loop, this is where the drawing happens
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.8f, 0.0f, 0.3f, 1.0f); // Acts as background color
        glClear(GL_COLOR_BUFFER_BIT);

        // tell the program which shader program is being used
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	
    glDeleteProgram(shaderProgram);
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}

// the key functions, ex. if you want to have your camera to move
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}