#version 330

out vec4 outputColor;

uniform float time;

void main()
{
        outputColor = vec4((sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f, 1.0);
}
