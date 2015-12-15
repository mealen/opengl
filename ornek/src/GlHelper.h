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
    GLuint textureBufferObject;
    GLuint vao;
    GLuint theProgram;
    GLuint texture;    
    GLuint vertexbuffer;
    float vertexPositions[12];
    float vertexColors[12];
    float textureCoords[6];

public:
    GlHelper();
    GLuint CreateShader(GLenum, const std::string &);
    GLuint CreateProgram(const std::vector<GLuint> &);
    void InitializeProgram();
    void reshape (int, int);

    void InitializeVertexBuffer();


    void render();
};

#endif
