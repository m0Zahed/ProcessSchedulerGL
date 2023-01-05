#include <iostream>
#include <fstream>

using namespace std;

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
    input.close();

    std::cout<< "The Following program is parsed\n " << program;

}
std::string vertexShaderString = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

int main(int argc, char* argv[])
{
    cout<< argc;
    parser(argv[1]);
    parser(argv[2]);
    cout << "Vector string\n" << vertexShaderString;
}