#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;

smooth out vec4 colorOut;
out vec2 TexCoord;

void main(){
    gl_Position = position;
    //colorOut = vec4(0.3,0.6,0.9,0.5);//color;
    colorOut = color;
    TexCoord = texCoord;
}
