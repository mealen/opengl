#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
uniform vec2 offset;
out vec4 colorOut;

void main(){
    vec4 totalOffset = vec4(offset.x, offset.y, 0.0, 0.0);
    gl_Position = position + totalOffset;
    //colorOut = vec4(0.3,0.6,0.9,0.5);//color;
    colorOut = color;
}
