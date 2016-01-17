#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 theColor;

uniform mat4 rotationMatrixM;
uniform mat4 scaleMatrixM;
uniform mat4 translateMatrixM;
uniform mat4 perspectiveMatrixM;

void main()
{
	vec4 cameraPos = translateMatrixM * rotationMatrixM * scaleMatrixM * position;
	mat4 translate = mat4(1);
	translate[2][3]= -2;
	cameraPos = cameraPos * translate;
	gl_Position = perspectiveMatrixM * cameraPos;
	theColor = color;
}

