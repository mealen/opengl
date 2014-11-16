#ifndef GL_HELPER_H
#define GL_HELPER_H

//gpl sheyi

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
    GLuint vao;
    GLuint theProgram;
    GLuint vertexbuffer;
    float vertexPositions[12];

public:
    GlHelper();
    GLuint CreateShader(GLenum, const std::string &);
    GLuint CreateProgram(const std::vector<GLuint> &);
    void InitializeProgram();
    void reshape (int, int);
    void APIENTRY DebugFunc(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const GLvoid*);

    void InitializeVertexBuffer();


    void render();
};


#endif
