#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include "VertexShaderManager.h"
#include "FragmentShaderManager.h"
#include "camera.h"
#include "Heightmap.h"
#include <vector>

// Callbacks
// For when the window is resized to adjust the viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Handle keyboard input for the window i.e press escape to close the window
void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window constraints 
const unsigned int SCR_WIDTH = 1920; // 800
const unsigned int SCR_HEIGHT = 1080; // 600

// Camera 
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Time
float deltaTime = 0.0f; // delta time between current and last frame
float lastFrame = 0.0f;

// lookat, perspective
// terrain class, shader manager

// Make shader manager for vertex shader and fragment shader
// We need terrain manager as well

// VAO - vertex array object
// VAO Acts as a 'wrapper around' all of the vertex buffer objects
// Important that we glBindVertexArray, this means select the VAO we want to use

// VBO - vertex buffer object

// Shader Managers
VertexShaderManager vertexShaderManager;
FragmentShaderManager fragmentShaderManager;
Heightmap heightMap;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Tell GLFW to capure the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    // Heightmaps paths
    // "resources/heightmaps/høydedata_svarthvitt.png"
    // "resources/heightmaps/høydedata.png"
    heightRet heightData = heightMap.initialize("høydedata_svarthvitt.png");

    if (heightData.vertices.empty()) {
        cout << "Failed to load the height map data" << endl;
        return -1;
    }
    else {
        std::cout << "Successfully loaded heightmap data!" << std::endl;
        std::cout << "Number of vertices: " << heightData.vertices.size() / 3 << std::endl;
        std::cout << "Number of indices: " << heightData.indices.size() << std::endl;
    }

    // Creates the OpenGL shader program
    GLuint shaderProgram = glCreateProgram();
    // Attach the Vertex and Fragment shaders to the shader program
    glAttachShader(shaderProgram, vertexShaderManager.getShaderId());
    glAttachShader(shaderProgram, fragmentShaderManager.getShaderId());
    // Link the shaders into the complete shader program to run on the GPU
    glLinkProgram(shaderProgram);

    // Generate a Vertex Array Object (VAO), a Vertex Buffer Object (VBO) and a Element Array Buffer Object
    GLuint terrainVAO, terrainVBO, terrainEBO;
    glGenVertexArrays(1, &terrainVAO);
    // Bind means that we are 'selecting the active buffer object' that we want to work with
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    // Bind or 'select' the VBO as the active buffer in the array buffer target
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    // Copy the vertex data into the buffer memory on the GPU
    glBufferData(GL_ARRAY_BUFFER, 
        heightData.vertices.size() * sizeof(float), // The size of the vertices buffer -> if too much change to int
        &heightData.vertices[0], 
        GL_STATIC_DRAW
    );

    // The Vertex attribute pointer tells us how to intepret the vertex data, as this will vary
    // Here each vertex has 3 GL_FLOAT values and that there is not additional data with them
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enables and sets the vertex array attribute array at index 0
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        heightData.indices.size() * sizeof(unsigned),
        &heightData.indices[0],
        GL_STATIC_DRAW
    );

    // Simulation loop, this is where the drawing happens
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle keyboard input
        processInput(window);

        glClearColor(0.1f, 0.01f, 0.1f, 1.0f); // Acts as background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and uses the selected color above
      
        glUseProgram(shaderProgram);
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 5000.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        // Tells the program which shader program is being used
        // The one we made previously
        //glUseProgram(shaderProgram);
        glBindVertexArray(terrainVAO); // Binds the VAO, making it the current active vertex array
        
        for (unsigned int strip = 0; strip < heightData.NUM_STRIPS; ++strip) {
            glDrawElements(GL_TRIANGLE_STRIP,
                heightData.NUM_VERTS_PER_STRIP,
                GL_UNSIGNED_INT,
                (void*)(sizeof(unsigned)
                    * (heightData.NUM_VERTS_PER_STRIP)
                    * strip));
        }


        glfwSwapBuffers(window); // Swaps the color buffers to display
        glfwPollEvents(); // Finally process events, like keyboard inputs
    }
	
    // Cleanup and release
    glDeleteProgram(shaderProgram);
    //glDeleteShader(vertexShader);
    //glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &terrainVAO);
    glDeleteBuffers(1, &terrainVBO);
    glDeleteBuffers(1, &terrainEBO);

    glfwTerminate();

    return 0;
}

// the key functions, ex. if you want to have your camera to move
void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime;

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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