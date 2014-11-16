//gpl sheyi
#include "GlHelper.h"

GLuint GlHelper::CreateShader(GLenum eShaderType, const std::string &strShaderFile) {
    GLuint shader = glCreateShader(eShaderType);
    std::string shaderCode;
    std::ifstream shaderStream(strShaderFile.c_str(), std::ios::in);
    if(shaderStream.is_open()){
            std::string Line = "";
            while(getline(shaderStream, Line))
                    shaderCode += "\n" + Line;
            shaderStream.close();
    }else{
            printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", strShaderFile.c_str());
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

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
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
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);
    return program;
}

void GlHelper::InitializeProgram() {
    std::vector<GLuint> shaderList;

    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "/home/engin/opengl-ab/3/vertex.shader"));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "/home/engin/opengl-ab/3/fragment.shader"));

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
    glEnableClientState(GL_COLOR_ARRAY) ;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}


GlHelper::GlHelper() {

    float vertexPositions[] = {
                -0.5f, -0.5f, 0.5f, 1.0f,
                 0.5f, -0.5f, 0.5f, 1.0f,
                 0.5f,  0.5f, 0.5f, 1.0f
    };
    
    float vertexColors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
    };

    std::copy(vertexPositions,vertexPositions + 12, this->vertexPositions);
    std::copy(vertexColors,vertexColors + 12, this->vertexColors);

    InitializeProgram();
    InitializeVertexBuffer();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}
void GlHelper::render() {
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(theProgram);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferObject);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glUseProgram(0);
}

void GlHelper::move(GlHelper::directions direction){
	float movementAmount = 0.1;

	switch (direction) {
	case UP:
		vertexPositions[1] += movementAmount;
		vertexPositions[5] += movementAmount;
		vertexPositions[9] += movementAmount;
		break;
	case DOWN:
		vertexPositions[1] -= movementAmount;
		vertexPositions[5] -= movementAmount;
		vertexPositions[9] -= movementAmount;
		break;
	case LEFT:
		vertexPositions[0] -= movementAmount;
		vertexPositions[4] -= movementAmount;
		vertexPositions[8] -= movementAmount;
		break;
	case RIGHT:
		vertexPositions[0] += movementAmount;
		vertexPositions[4] += movementAmount;
		vertexPositions[8] += movementAmount;
		break;
	}

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY) ;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

