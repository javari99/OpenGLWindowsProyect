
#define PROG_VERSION "0.0.1"


int window_width = 1920;
int window_height = 1080;

bool Wireframe_mode = false;

const char* WINDOW_TITLE = "OpenGL Project - Javier Arin";

#include "BoilerPlateStaticIncludes.h"

#include "Shader.hpp"

#include <sstream>
#include "JavUtils.h"
#include "TimeManager.h"
#include "STB/stb_image.h"

#include <fstream>

float mixValue = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* initializeGLFW();
GLFWwindow* initialize();

int main(int argc, const char * argv[]) {
    GLFWwindow* window = initialize();
    if(!window){
        JUtils::printError("An error ocurred while loading GLFW or GLAD");
        return -1;
    }
    
    //Load the only shader we have for now
    Shader shaderProgram("Shaders/vertexShader.vs","Shaders/fragmentShader.fs");
    
    
    //---------------Lets draw a triangle!------------------
    //                      Positions          Color            UV
    float vertices[] = { 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    unsigned int indices[] = { 0, 1, 3,
                               1, 2, 3};
    // Create a buffer for the vertex data into the gpu
    unsigned int VBO, EBO, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);
    
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the vertex information buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // fill it with information
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO); // bind the element buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //fill it
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    // Add the vertex information to location 0 for shaders to use
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Now add the color information
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // Add Texture coordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    // Bind the Elements buffer, VAO will remember only buffer binds to element_array_buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    // Once we have finished with the load of the different features we should bind back
    // to 0 so we dont modify accidentaly other VAOs
    glBindVertexArray(0);

    
    // ------------------- Textures -------------------
    stbi_set_flip_vertically_on_load(true);
    // load texture using stbi library
    int width, height, nrChannels;
    unsigned char *data = stbi_load("Textures/containerWood.jpg", &width, &height, &nrChannels, 0);
    // generate texture on the GPU
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set the texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Upload texture to the GPU
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Generates the mipmap
    }else{
        JUtils::printError("Failed to load texture 1!");
    }
    
    // Free image data as it has been uploaded to the GPU
    stbi_image_free(data);
    
    // Let's load the second texture
    data = stbi_load("Textures/wallBrick.jpg", &width, &height, &nrChannels, 0);
    GLuint texture2;
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Set the texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Generates the mipmap
    }else{
        JUtils::printError("Failed to load texture 2!");
    }
    stbi_image_free(data);
    //shaderProgram.use();
    
    //set up time for physics calculations.
    TimeManager timeManager;
    float DeltaTime = 0;
    // ------------------------------------
    //          Main window loop
    // ------------------------------------
    while(!glfwWindowShouldClose(window)){
        //first set up time timer
        timeManager.StartPoint();
        if(timeManager.getDeltaTime()!=0){
            DeltaTime = timeManager.getDeltaTime();
            std::stringstream ss;
            ss << WINDOW_TITLE << " ProcessTime: " << std::setw(10) <<DeltaTime << " ms";
            glfwSetWindowTitle(window, ss.str().c_str());
        }
        // The main loop should go as follows:
        // 1. Get input
        // 2. Render
        // 3. Check events and swap buffers
        // -- get input
        processInput(window);
        
        // ----- render -----
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // set the active texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        
        // Here we will draw what we want. To draw something we should
        // tell openGL which program to use when drawing and bind the VAO
        // this order is important
        shaderProgram.use();
        shaderProgram.setInt("TexSampler1", 0);
        shaderProgram.setInt("TexSampler2", 1);
        shaderProgram.setFloat("mixValue", mixValue);
        
        //We will make a transformation
        for(int i = 0; i < 2; i++){
            glm::mat4 trans(1.0f);
            if(i == 0){
                trans = glm::translate(trans,glm::vec3(0.5f,0.0f,0.0f));
                trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));
                
            }else{
                trans = glm::translate(trans, glm::vec3(-0.5, 0.5f, 0.0f));
                trans = glm::scale(trans,glm::vec3(abs(sin(glfwGetTime()))));
            }
            
            shaderProgram.setMat4("transform", trans);
        
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        
        // -- Check events and swap buffer
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Set endpoint for time calculation
        timeManager.FinishPoint();
    }
    
    std::cout << "Exiting the program. Cleanup Starting"<< std::endl;
    
    // Delete all the memory assigned
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
    glDeleteTextures(1,&texture);
    glDeleteTextures(1,&texture2);
    
    
    // We finish the program by finishing the used subsystems
    glfwTerminate();
    return 0;
}

// Function that handles the resize of the GLFW window
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    window_width = width;
    window_height = height;
    glViewport(0,0,width,height);
}

// Function defined to process the input
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        Wireframe_mode = !Wireframe_mode;
        if (Wireframe_mode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_R)) mixValue += 0.01f;
    if(glfwGetKey(window, GLFW_KEY_F)) mixValue -= 0.01f;
    mixValue = JUtils::clampF(0, 1, mixValue);
    
}

// Initializing GLFW and setting the propper flags
GLFWwindow* initializeGLFW(){
    
    std::cout << "Initializing GLFW:3.3" << std::endl;
    if(!glfwInit()){
        JUtils::printError("Could not initialize GLFW");
        return NULL;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Set up code for the window
    std::cout << "Setting up window" << std::endl;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, WINDOW_TITLE, NULL, NULL);
    return window;
}

// Global initialize function
GLFWwindow* initialize(){
    
    std::cout << "JavEngine - Version: " << PROG_VERSION << std::endl;
    //Initialize GLFW and the window
    GLFWwindow* window = initializeGLFW();
    if(window==NULL){
        JUtils::printError("Could not initialize GLFW window");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    //GLFW is 60fps capped by default, uncommenting this will remove the V-sync
    //glfwSwapInterval(0);
    // We now init GLAD
    std::cout << "Initializing GLAD" << std::endl;
    if( !gladLoadGLLoader( ((GLADloadproc)glfwGetProcAddress) ) ){
        glfwTerminate();
        JUtils::printError("GLad was unable to load");
        return NULL;
    }
    
    // We set the viewport
    glViewport(0, 0, window_width, window_height);
    
    // Set the callback functions for the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}
