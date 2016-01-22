#ifndef GL_HELPER_H
#define GL_HELPER_H

#include <GL/glew.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <tgmath.h>
#include <GLFW/glfw3.h>

#ifdef LOAD_X11
#define APIENTRY
#endif



class GlHelper {

private:
	int width, height;

    GLuint positionBufferObject;
    GLuint colorBufferObject;
    GLuint indexBufferObject;
    GLuint vao;
    GLuint theProgram;
    GLuint vertexbuffer;
    GLuint offsetLocation;
    float xOffset;
    float yOffset;
    float vertexPositions[24 * 4];
    float vertexColors[24 * 4];
    unsigned short vertexIndex[36];
    float perspectiveMatrix[16];
    float lookAtMatrix[16];
    float translationMatrix[16];
    float scaleMatrix[16];
    float rotationMatrix[16];
    GLuint perspectiveMatrixM;
    GLuint lookAtMatrixM;
    GLuint rotationMatrixM;
    GLuint scaleMatrixM;
    GLuint translateMatrixM;
	float rotationAngle;


public:
    enum directions { UP, LEFT, DOWN, RIGHT };

    GlHelper(int,int);
    GLuint CreateShader(GLenum, const std::string &);
    GLuint CreateProgram(const std::vector<GLuint> &);
    void InitializeProgram();
    void reshape (int, int);
    void processInput(directions);
    void scale (float, float, float);
    void translate(float, float, float);
	void rotate(float,float,float,float);
    void InitializeVertexBuffer();

    void cameraMove(float,float);
    void render();
};

#endif
