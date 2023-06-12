#include <exception>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "shader.h"
#include "shapes.h"
#include "stb_image.h"


#define CHANNEL_TYPE(CHNL) CHNL

//to compile - g++ test.cpp glad.c -o test -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    
    // (left, bottom, left, top) size of the window
    glViewport(0, 0, width, height);
}

static int textureCount = 0;
unsigned int initTexture(unsigned int& texture, const char* fileName)
{
    // -- Initalizing Texture --
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    textureCount++;

    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    
    if (data)
    {
        std::cout << "Successfully loaded texture" << std::endl;
        // specifiy that the image data is RGBA
        unsigned int CHANNEL = GL_RGB;
        if(nrChannels > 3)
        {
            CHANNEL = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,CHANNEL , GL_UNSIGNED_BYTE, data);
        #undef CHANNEL
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;


int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //initiating OpenGL Context
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL",NULL,NULL);
    if(window == NULL)
    {
        std::cout <<"Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //GLAD doing its work -- Checking to see if GLAD has properly loaded
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Frame size adjuster calllback function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // --- Shapes Data ----
    float Vertices[] = {
    // positions                              //color                         //texture      
    0.5f, 0.5f, 0.0f,        1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };
    float triangleVertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
        0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
       
    };
    float rectVertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    int indices[] = {
        0,1,3,
        1,2,3
    };


    // -- Shader Object --
    Shader s1(argv[1],argv[2]);

    // --Shapes Object --
    Shapes Tri(Vertices, sizeof(Vertices));
    Shapes Rect(Vertices,indices,sizeof(Vertices),sizeof(indices));
    Rect.initTextureRectangle();


    // -- Initialize Textures --
    unsigned int text1 = initTexture(text1, "awesomeface.png");
    unsigned int text2 = initTexture(text2, "container.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, text1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, text2);
    s1.use();
    s1.setInt("texture1", 0);
    s1.setInt("texture2", 1);

    // --- Render Loop ---
    while(!glfwWindowShouldClose(window))
    {   
        
        processInput(window);
        
        glClearColor(0.07f,0.23f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // -- rendering commands --  
        
        glBindVertexArray(VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

        
        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }
    
    //unbind vao and vbo array and texture
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1,&text1);
    glDeleteBuffers(1,&text2);
    
    glfwTerminate();
    return 0;
}   
