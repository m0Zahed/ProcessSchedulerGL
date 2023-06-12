#version 330 core
out vec4 FragColor;

in vec2 textCoord;
in vec3 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1,textCoord),texture(texture2,textCoord),0.5);

}