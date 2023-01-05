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

const char* fragmentShaderString = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(ourColor,1.0);\n"
"}\0";
const char* vertexShaderString = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
    "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "ourColor = aColor;\n"
    "}\0";


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

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
unsigned int shaderProgram;
void CreateShaderProgram()
{


    shaderProgram = glCreateProgram();
    // Vertex Shader and fragment shader Instantiation -- 
    unsigned int vertexShader;
    unsigned int fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // -- GLSL source code --
    glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
    glCompileShader(fragmentShader);
    //shaderCompileResult(fragmentShader);
    glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
    glCompileShader(vertexShader);
    //shaderCompileResult(vertexShader);

    //Attaching and linking the programs
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //Checking of the GLSL Shader source code compiled properly
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
        infoLog << std::endl;
    }

    glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		printf("Error validating program: '%s'\n", infoLog);
	}
   
    //Next delete the shader item  objects since we do not need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return;
}

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

    //registering the callback function in the OpenGL context
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //initalizing glsl code
    //Shader s1(argv[1],argv[2]);
    CreateShaderProgram();

     //Initializing Shapes
    float triangleVertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
        0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
       
    };
    Shapes Tri(triangleVertices, sizeof(triangleVertices));
    
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
    Shapes Rect(rectVertices,indices,sizeof(rectVertices),sizeof(indices));
    
    //initialize the required shaped
    Tri.initTriangle();
    

    //render loop
    while(!glfwWindowShouldClose(window))
    {   

        processInput(window);
        
        //Clearing the color once
        glClearColor(0.75f,0.3f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        // -- rendering commands --
         //use shader program to render object
        glUseProgram(shaderProgram);
            // -- Code when the fragment shader has a uniform --
            // float timeValue = glfwGetTime();
            // float greenValue = (sin(timeValue)/2.0f)+0.5f;
            // int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
            // glUniform4f(vertexColorLocation,0.0f,greenValue,0.0,1.0f);
            glBindVertexArray(VAO);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
            glBindVertexArray(0);
        glUseProgram(0);
        //polss events such as keyboard keys and mouse movements and raises flags 
        glfwPollEvents();
        //continously swapbuffer
        glfwSwapBuffers(window);

        
        
    }
    
    //unbind vao and vbo array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
    glfwTerminate();
    return 0;
}   