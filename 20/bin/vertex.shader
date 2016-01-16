#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 theColor;

uniform mat4 rotationMatrixM;
uniform mat4 perspectiveMatrixM;

void main()
{
	vec4 cameraPos = rotationMatrixM * position;
	gl_Position = perspectiveMatrixM * cameraPos;
	theColor = color;
}

