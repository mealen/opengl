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
    float xOffset;
    float yOffset;
    float vertexPositions[36 * 4];
    float vertexColors[36 * 4];
    GLuint frustumScaleUnif;
    GLuint zNearUnif;
    GLuint zFarUnif;

public:
    enum directions { UP, LEFT, DOWN, RIGHT };

    GlHelper();
    GLuint CreateShader(GLenum, const std::string &);
    GLuint CreateProgram(const std::vector<GLuint> &);
    void InitializeProgram();
    void reshape (int, int);
    void move(directions);

    void InitializeVertexBuffer();


    void render();
};

#endif
