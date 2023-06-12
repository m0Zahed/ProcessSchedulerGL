#include "shapes.h"

//Type inferred to be Triangle
Shapes::Shapes(float* data, int datasize) : dataSize(datasize)
{   
    type = "Triangle";
    for(int i = 0; i < datasize;i++)
    {
        vertices.push_back(*(data + i));
    }
}

//Type inferred to be Rectangle
Shapes::Shapes(float* data, int* index, int dataSizeData, int dataSizeIndex) : dataSize(dataSizeData), indexSize(dataSizeIndex)
{
    type = "Rectangle";
    for(int i = 0; i < dataSizeData;i++)
    {
        vertices.push_back(*(data + i));
    }
    for(int i = 0; i < dataSizeIndex;i++)
    {
        indices.push_back(*(index+ i));
    }
}

void Shapes::initTextureTriangle(){
    float vertexArray[dataSize];
    int iterator = 0;
    for(auto var : vertices)
    {   
        vertexArray[iterator] = var; 
        iterator++;
    }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Shapes::initTriangle(){

    float vertexArray[dataSize];
    int iterator = 0;
    for(auto var : vertices)
    {   
        vertexArray[iterator] = var; 
        iterator++;
    }

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Shapes::initRectangle(){
    //initialize VBO and VAO and then bind VBO
    float vertexArray[dataSize];
    int indexArray[indexSize];
    int iterator = 0;
    for(auto var : vertices)
    {   
        vertexArray[iterator] = var; 
        iterator++;
    }
    iterator = 0;
    for(auto var : indices)
    {   
        indexArray[iterator] = var; 
        iterator++;
    }

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Shapes::initTextureRectangle()
{
    float vertexArray[dataSize];
    int indexArray[indexSize];
    // -- convert vector type to array
    int iterator = 0;
    for(auto var : vertices)
    {   
        vertexArray[iterator] = var; 
        iterator++;
    }
    iterator = 0;
    for(auto var : indices)
    {   
        indexArray[iterator] = var; 
        iterator++;
    }

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}