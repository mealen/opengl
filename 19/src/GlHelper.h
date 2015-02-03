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
#include <cmath>
#include <GLFW/glfw3.h>

#ifdef LOAD_X11
#define APIENTRY
#endif

class GlHelper {

private:
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
    float translateMatrix[16];
    float translateMatrix2[16];
    float translateMatrix3[16];
    GLuint perspectiveMatrixM;
    GLuint translateMatrixM;

public:
    enum directions { UP, LEFT, DOWN, RIGHT };

    GlHelper();
    GLuint CreateShader(GLenum, const std::string &);
    GLuint CreateProgram(const std::vector<GLuint> &);
    void InitializeProgram();
    void reshape (int, int);
    void scale (double, double);
    void move(directions);

    void InitializeVertexBuffer();

    void render();
};

#endif
