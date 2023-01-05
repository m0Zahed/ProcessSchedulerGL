#ifndef C721A9BD_735C_4777_A53E_099E323BA9DB
#define C721A9BD_735C_4777_A53E_099E323BA9DB

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <string>

extern unsigned int VBO,VAO,EBO;

class Shapes
{
    public:
    
    std::string type;
    std::vector<float> vertices;
    std::vector<int> indices;
    int dataSize, indexSize;
    Shapes(float* data, int dataSize);
    Shapes(float* data, int* indices, int dataSizeData, int dataSizeIndex);
    void initTriangle();
    void initRectangle();
};

#endif /* C721A9BD_735C_4777_A53E_099E323BA9DB */
