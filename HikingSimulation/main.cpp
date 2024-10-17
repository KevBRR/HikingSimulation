#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexShaderManager.h"
#include "FragmentShaderManager.h"

// For when the window is resized to adjust the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Handle keyboard input for the window i.e press escape to close the window
void processInput(GLFWwindow* window);

// lookat, perspective
// terrain class, shader manager

// Make shader manager for vertex shader and fragment shader
// We need terrain manager as well

// VAO - vertex array object
// VAO Acts as a 'wrapper around' all of the vertex buffer objects
// Important that we glBindVertexArray, this means select the VAO we want to use

// VBO - vertex buffer object

// Window constraints 
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shader Managers
VertexShaderManager vertexShaderManager;
FragmentShaderManager fragmentShaderManager;

// Heightmaps paths
// "resources/heightmaps/høydedata_svarthvitt.png"
// "resources/heightmaps/høydedata.png"

int main()
{
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return -1;
    }

    // Creating the window and applying the OpenGL context to this window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hiking Simulation", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initializes GLEW, for the ability to access the functions with it
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

    // Creates the OpenGL shader program
    GLuint shaderProgram = glCreateProgram();
    // Attach the Vertex and Fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShaderManager.getShaderId());
    glAttachShader(shaderProgram, fragmentShaderManager.getShaderId());
    // Link the shaders into the complete shader program to run on the GPU
    glLinkProgram(shaderProgram);

    // Vertex data
    // An array of three vertices in 3D space, ultimately forming a triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left vertex position
         0.5f, -0.5f, 0.0f, // right vertex position
         0.0f,  0.5f, 0.0f  // top vertex position
    };

    // Generate a Vertex Array Object (VAO) and a Vertex Buffer Object (VBO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind means that we are 'selecting the active buffer object' that we want to work with
    glBindVertexArray(VAO);
    // Bind or 'select' the VBO as the active buffer in the array buffer target
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy the vertex data into the buffer memory on the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // The Vertex attribute pointer tells us how to intepret the vertex data, as this will vary
    // Here each vertex has 3 GL_FLOAT values and that there is not additional data with them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enables and sets the vertex array attribute array at index 0
    glEnableVertexAttribArray(0);

    // Simulation loop, this is where the drawing happens
    while (!glfwWindowShouldClose(window))
    {
        // Handle keyboard input
        processInput(window);

        glClearColor(0.8f, 0.0f, 0.3f, 1.0f); // Acts as background color
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen and uses the selected color above

        // Tells the program which shader program is being used
        // The one we made previously
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // Binds the VAO, making it the current active vertex array
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draws the vertices as a triangle

        glfwSwapBuffers(window); // Swaps the color buffers to display
        glfwPollEvents(); // Finally process events, like keyboard inputs
    }
	
    // Cleanup and release
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