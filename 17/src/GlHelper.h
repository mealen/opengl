#ifndef GL_HELPER_H
#define GL_HELPER_H

#include <GL/glew.h>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

#ifdef LOAD_X11
#define APIENTRY
#endif

class GlHelper {

private:
    GLuint positionBufferObject;
    GLuint colorBufferObject;
    GLuint vao;
    GLuint theProgram;
    GLuint vertexbuffer;
    GLuint offsetLocation;
    float origX;
    float origY;
    float xOffset;
    float yOffset;
    float zOffset;
    float vertexPositions[12];
    float vertexColors[12];

public:
    enum directions { UP, LEFT, DOWN, RIGHT };

    GlHelper();
    GLuint CreateShader(GLenum, const std::string &);
    GLuint CreateProgram(const std::vector<GLuint> &);
    void InitializeProgram();
    void reshape (int, int);
    void zmove (double, double);
    void mmove (double, double);
    void move(directions);

    void InitializeVertexBuffer();


    void render();
};

#endif
