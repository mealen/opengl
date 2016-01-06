//gpl sheyi
#include "GlHelper.h"

GLuint GlHelper::CreateShader(GLenum eShaderType, const std::string &strShaderFile) {
    GLuint shader = glCreateShader(eShaderType);
    std::string shaderCode;
    std::ifstream shaderStream(strShaderFile.c_str(), std::ios::in);

    if(shaderStream.is_open()) {
        std::string Line = "";

        while(getline(shaderStream, Line))
            shaderCode += "\n" + Line;

        shaderStream.close();
    } else {
        std::cerr << strShaderFile.c_str() << " dosyasi acilamadi. Uygulamanizi bu dosyanin oldugu dizinde calistirdiginizdan emin olunuz." << std::endl;
        getchar();
        return 0;
    }

    const char* shaderCodePtr = shaderCode.c_str();
    glShaderSource(shader, 1, &shaderCodePtr, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        const char *strShaderType = NULL;

        switch(eShaderType)
        {
        case GL_VERTEX_SHADER:
            strShaderType = "vertex";
            break;

        case GL_GEOMETRY_SHADER:
            strShaderType = "geometry";
            break;

        case GL_FRAGMENT_SHADER:
            strShaderType = "fragment";
            break;
        }

        std::cerr << strShaderType << " tipi shader dosyasi derlenemedi. Detaylar:\n" << strInfoLog << std::endl;
        delete[] strInfoLog;

    }

    return shader;
}

GLuint GlHelper::CreateProgram(const std::vector<GLuint> &shaderList) {
    GLuint program = glCreateProgram();

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glAttachShader(program, shaderList[iLoop]);

    glLinkProgram(program);
    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        std::cerr << "Linkleme sorunu: \n" << strInfoLog << std::endl;
        delete[] strInfoLog;
    }

    offsetLocation = glGetUniformLocation(program, "offset");
    perspectiveMatrixM = glGetUniformLocation(program, "perspectiveMatrixM");
    scaleMatrixM = glGetUniformLocation(program, "scaleMatrixM");

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);

    return program;
}

void GlHelper::InitializeProgram() {
    std::vector<GLuint> shaderList;

    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "./vertex.shader"));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "./fragment.shader"));


    theProgram = CreateProgram(shaderList);
    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void GlHelper::InitializeVertexBuffer() {
    glGenBuffers(1, &positionBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY) ;
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //coloring
    glGenBuffers(1, &colorBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferObject) ;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors,GL_STATIC_DRAW);
    glColorPointer(4, GL_FLOAT, 0, 0) ;
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableClientState(GL_COLOR_ARRAY) ;
    
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndex), vertexIndex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

GlHelper::GlHelper() {

    float vertexPositions[] = {
        +1.0f, +1.0f, +1.0f, 1.0f,
        -1.0f, -1.0f, +1.0f, 1.0f,
        -1.0f, +1.0f, -1.0f, 1.0f,
        +1.0f, -1.0f, -1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f, 1.0f,
        +1.0f, +1.0f, -1.0f, 1.0f,
        +1.0f, -1.0f, +1.0f, 1.0f,
        -1.0f, +1.0f, +1.0f, 1.0f,
    };
    float vertexColors[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f,
    };
    unsigned short vertexIndex[] = {
        0, 1, 2,
        1, 0, 3,
        2, 3, 0,
        3, 2, 1,

        5, 4, 6,
        4, 5, 7,
        7, 6, 4,
        6, 7, 5,
    };

    xOffset = 0.0f;
    yOffset = 0.0f;

    std::copy(vertexPositions,vertexPositions + 8*4, this->vertexPositions);
    std::copy(vertexColors,vertexColors + 8*4, this->vertexColors);
    std::copy(vertexIndex,vertexIndex + 8*3, this->vertexIndex);

    float fFrustumScale=1.0f, zFar=60.0f,zNear=1.0f;
    memset(perspectiveMatrix,0,16*sizeof(float));
    perspectiveMatrix[0]=fFrustumScale;
    perspectiveMatrix[5]=fFrustumScale;
    perspectiveMatrix[10]= (zFar + zNear) / (zNear - zFar);
    perspectiveMatrix[14]= (2*zFar * zNear) / (zNear - zFar);
    perspectiveMatrix[11]= -1.0f;

    memset(scaleMatrix,0,16*sizeof(float));
    scaleMatrix[0] = 4.0f;
    scaleMatrix[5] = 4.0f;
    scaleMatrix[10]= 4.0f;
    scaleMatrix[12]= -10.0f;
    scaleMatrix[13]= -10.0f;
    scaleMatrix[14]= -25.0f;
    scaleMatrix[15]= 1;

    memset(scaleMatrix2,0,16*sizeof(float));
    scaleMatrix2[12]= 10.0f;
    scaleMatrix2[13]= 10.0f;
    scaleMatrix2[14]= -25.0f;
    scaleMatrix2[15]= 1;
    
    InitializeProgram();
    InitializeVertexBuffer();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferObject);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

    glBindVertexArray(0);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

}
void GlHelper::render() {
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(theProgram);
    glBindVertexArray(vao);

    glUniform2f(offsetLocation, xOffset, yOffset);
    glUniformMatrix4fv(perspectiveMatrixM, 1,GL_FALSE,perspectiveMatrix);
    glUniformMatrix4fv(scaleMatrixM, 1, GL_FALSE, scaleMatrix);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT,0);

    //std::cout << fFrustumScale ;
    float timeVar = sin(glfwGetTime()) * 2.0f + 2.0f;
    scaleMatrix2[0] = timeVar, scaleMatrix2[5] = timeVar, scaleMatrix2[10] = timeVar;
    glUniformMatrix4fv(scaleMatrixM, 1, GL_FALSE, scaleMatrix2);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT,0);
    
    glBindVertexArray(0);
    glUseProgram(0);
}

void GlHelper::move(GlHelper::directions direction) {
    float movementAmount = 0.1;

    switch (direction) {
    case UP:
        yOffset += movementAmount;
        break;

    case DOWN:
        yOffset -= movementAmount;
        break;

    case LEFT:
        xOffset -= movementAmount;
        break;

    case RIGHT:
        xOffset += movementAmount;
        break;
    }

}

void GlHelper::reshape (int w, int h)
{
    perspectiveMatrix[0] = 1.0f / (w / (float)h);
    perspectiveMatrix[5] = 1.0f;
    
    glUseProgram(theProgram);
    glUniformMatrix4fv(perspectiveMatrixM, 1, GL_FALSE, perspectiveMatrix);
    glUseProgram(0);
    
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}


void GlHelper::scale(double x, double y) {
    scaleMatrix[0] += y / 10;
    scaleMatrix[5] += y / 10;
    scaleMatrix[10] += y / 10;
}
