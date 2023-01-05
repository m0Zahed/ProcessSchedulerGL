#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>

//VBO and VAO ids
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
//shader Program instantiation
unsigned int shaderProgram;


//vertex shader code in GLSL
// const char* vertexShaderString = "#version 330 core\n"
// "layout (location = 0) in vec3 aPos;\n"
// "void main()\n"
// "{\n"
// " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
// "}\0";
// const char* fragmentShaderString = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//         "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//     "}\0";
const char* vertexShaderSource;
const char* fragmentShaderSource;
std::string fragmentShader,vertexShader;


void parser(const std::string& location)
{   
    std::ifstream input  (location, std::ifstream::in);

    if(!(input.is_open()))
    {
        std::cout << "Didn't open the input file properly, exiting now";
        return;
    }

    std::string line, program; 
    while(input)        // parsing vector shader into a single single
    {
        line = "";
        getline(input, line);
        program.append(line+"\n");
        
    }
    program.append("\0");
    std::cout<< "The Following program is parsed " << program;
    input.close();

    if( location == "vertexShader.txt")
    {
        vertexShader = program;//copy data into seperate buffer because the file stream data only exits while the file is open and gets deleted when the file closes so that char* pointer points to nothing
        vertexShaderSource = &vertexShader[0];
    }
    else 
    {
        fragmentShader  = program;
        fragmentShaderSource = &fragmentShader[0];
    }
}


// If escape keypress close window
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

//to compile - g++ test.cpp -o test -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    
    // (left, bottom, left, top) size of the window
    glViewport(0, 0, width, height);
}

void CreateRectangle()
{
    //initialize VBO and VAO and then bind VBO
    float vertices[] = {
        0.5f, 0.5f, 0.0f, // top right
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f // top left
    };
    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };
    
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ==>  After the vertex shaders and fragments shader data is loaded into 
    //the memory of the gpu, we can now tell openGL how its organised so it can access it.
    // (The starting layout location defined in the shader program, Number of attributes, size of each attribute, Not Important rn, offset to next vertex, starting location address)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}
void CreateTriangle()
{
    //initialize VBO and VAO and then bind VBO
    float vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
        0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
       
    };
   
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //After the vertex shaders and fragments shader data is loaded into 
    //the memory of the gpu, we can now tell openGL how its organised so it can access it.
    // (The starting layout location defined in the shader program, Number of attributes, size of each attribute, Not Important rn, offset to next vertex, starting location address)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    

}

void shaderCompileResult(unsigned int theShader)
{
    GLint result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the shader: '%s'\n", eLog);
		return;
	}
}

void CreateShaderProgram()
{


    shaderProgram = glCreateProgram();
    // Vertex Shader and fragment shader Instantiation -- 
    unsigned int vertexShader;
    unsigned int fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // -- GLSL source code --
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    shaderCompileResult(fragmentShader);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    shaderCompileResult(vertexShader);

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

    //initalizing glsl code
    parser(argv[1]);
    parser(argv[2]);
    
    //registering the callback function in the OpenGL context
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    CreateTriangle();
    //CreateRectangle();
    CreateShaderProgram();
    
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