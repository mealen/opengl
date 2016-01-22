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
    lookAtMatrixM = glGetUniformLocation(program, "lookAtMatrixM");
    rotationMatrixM = glGetUniformLocation(program, "rotationMatrixM");
    scaleMatrixM = glGetUniformLocation(program, "scaleMatrixM");
    translateMatrixM = glGetUniformLocation(program, "translateMatrixM");

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

void GlHelper::scale (float x, float y, float z) {
	scaleMatrix[0] = x;
	scaleMatrix[5] = y;
	scaleMatrix[10] = z;
}

void GlHelper::translate(float x, float y, float z) {
	translationMatrix[12] = x;
	translationMatrix[13] = y;
	translationMatrix[14] = z;
}

void GlHelper::rotate(float x, float y, float z, float angle){
	rotationMatrix[0]  = x*x + (1 - x*x ) * cos(angle);
	rotationMatrix[1]  = (1 - cos(angle))*x*y - z*sin(angle);
	rotationMatrix[2]  = (1 - cos(angle))*x*z + y*sin(angle);
	rotationMatrix[4]  = (1 - cos(angle))*x*y + z*sin(angle);
	rotationMatrix[5]  = y * y + (1 -y*y) * cos(angle);
	rotationMatrix[6]  = (1 - cos(angle))*y*z - x*sin(angle);
	rotationMatrix[8]  = (1 - cos(angle))*x*z - y*sin(angle);
	rotationMatrix[9]  = (1 - cos(angle))*y*z + x*sin(angle);
	rotationMatrix[10] = z * z + (1 - z*z) * cos(angle);

}


GlHelper::GlHelper(int width, int height): width(width), height(height) {

    rotationAngle = 0;


    float vertexPositions[] = {
        +0.05f, +0.05f,  0.05f, 1.0f,
        -0.05f, -0.05f,  0.05f, 1.0f,
        -0.05f, +0.05f, -0.05f, 1.0f,
        +0.05f, -0.05f, -0.05f, 1.0f,

        -0.05f, -0.05f, -0.05f, 1.0f,
        +0.05f, +0.05f, -0.05f, 1.0f,
        +0.05f, -0.05f,  0.05f, 1.0f,
        -0.05f, +0.05f,  0.05f, 1.0f,
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

    float fFrustumScale=2.4f, zFar=3.0f, zNear=1.0f;
    memset(perspectiveMatrix,0,16*sizeof(float));
    perspectiveMatrix[0]=fFrustumScale / (width / (float)height);;
    perspectiveMatrix[5]=fFrustumScale;
    perspectiveMatrix[10]= (zFar + zNear) / (zNear - zFar);
    perspectiveMatrix[14]= (2*zFar * zNear) / (zNear - zFar);
    perspectiveMatrix[11]= -1.0f;


    memset(lookAtMatrix,0,16*sizeof(float));
    lookAtMatrix[0] = 1;
    lookAtMatrix[5] = 1;
    lookAtMatrix[10] = 1;
    lookAtMatrix[15] = 1;

    memset(rotationMatrix,0,16*sizeof(float));
	rotationMatrix[0] = 1;
	rotationMatrix[5] = 1;
	rotationMatrix[10] = 1;
	rotationMatrix[15] = 1;

	memset(translationMatrix,0,16*sizeof(float));
	translationMatrix[0] = 1;
	translationMatrix[5] = 1;
	translationMatrix[10] = 1;
	translationMatrix[15] = 1;

	memset(scaleMatrix,0,16*sizeof(float));
	scaleMatrix[0] = 1;
	scaleMatrix[5] = 1;
	scaleMatrix[10] = 1;
	scaleMatrix[15] = 1;

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
    glUniformMatrix4fv(lookAtMatrixM, 1,GL_FALSE,lookAtMatrix);



    float modTimeVar = fmod(glfwGetTime(),2 *M_PI);
    float sinTimeVar = sin(glfwGetTime() ) * 0.5;
    float cosTimeVar = cos(glfwGetTime() ) * 0.5;
    //float cosTimeVar = cos(glfwGetTime() ) * 0.005f;
    //std::cout << "sinTimeVar is " << sinTimeVar << std::endl;
	rotate(0.707106781186548, 0.707106781186548, 0, modTimeVar);
    glUniformMatrix4fv(rotationMatrixM, 1, GL_FALSE, rotationMatrix);
	scale(1,1,1);
    glUniformMatrix4fv(scaleMatrixM, 1, GL_FALSE, scaleMatrix);
    translate(cosTimeVar,sinTimeVar,0);
    glUniformMatrix4fv(translateMatrixM, 1, GL_FALSE, translationMatrix);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT,0);

    rotate(0, 0, 0, 0);
	glUniformMatrix4fv(rotationMatrixM, 1, GL_FALSE, rotationMatrix);
    scale(sinTimeVar + 1,sinTimeVar + 1,sinTimeVar + 1);
	glUniformMatrix4fv(scaleMatrixM, 1, GL_FALSE, scaleMatrix);

    translate(0.5,0.5,0);
    glUniformMatrix4fv(translateMatrixM, 1, GL_FALSE, translationMatrix);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT,0);

    translate(-0.5,-0.5,0);
    glUniformMatrix4fv(translateMatrixM, 1, GL_FALSE, translationMatrix);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT,0);

    scale(1,1,1);
    glUniformMatrix4fv(scaleMatrixM, 1, GL_FALSE, scaleMatrix);

    rotate(0.707106781186548, 0.707106781186548, 0, modTimeVar);
    glUniformMatrix4fv(rotationMatrixM, 1, GL_FALSE, rotationMatrix);
    translate(0.5,-0.5,0);
    glUniformMatrix4fv(translateMatrixM, 1, GL_FALSE, translationMatrix);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT,0);

    translate(-0.5,0.5,0);
    glUniformMatrix4fv(translateMatrixM, 1, GL_FALSE, translationMatrix);
    glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT,0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void GlHelper::processInput(GlHelper::directions direction) {

    switch (direction) {
    case UP:
		rotationAngle += 0.1;
        break;

    case DOWN:
		rotationAngle -= 0.1;
        break;

    case LEFT:
        //rotationMatrix[12] -= movementAmount;
        break;

    case RIGHT:
        //rotationMatrix[12] += movementAmount;
        break;
    }
	
	//rotate(rotationAngle);

}

void crossProduct(float a, float b, float c,
				  float x, float y, float z,
				  float &k, float &l, float &m){
	k=b*z-c*y;
	l=c*x-a*z;
	m=a*y-b*x;
}

void GlHelper::cameraMove(float xi,float yi){
	float lookAtX,lookAtY, lookAtZ=1;
	lookAtX = (xi-(width/2))/1000;
	lookAtY = (yi-(height/2))/1000;
	std::cout << "xChange: " << lookAtX << ", yChange: " << lookAtY << std::endl;
	float lenght = sqrt(lookAtX * lookAtX + lookAtY * lookAtY + lookAtZ*lookAtZ);
	lookAtX = lookAtX / lenght;
	lookAtY = lookAtY / lenght;
	lookAtZ = lookAtZ / lenght;


	//look at coordinate is 0,0,-1 +x +y
	//default up is 0,1,0
	float rightA,rightB,rightC;
	//std::cout << "right was " << rightA <<"," << rightB <<"," << rightC << std::endl;
	crossProduct(lookAtX,lookAtY,lookAtZ,0,1,0, rightA,rightB,rightC);
	//std::cout << "right is " << rightA <<"," << rightB <<"," << rightC << std::endl;
	//now calculate up with change
	float upA,upB,upC;
	crossProduct(lookAtX,lookAtY,lookAtZ,rightA,rightB,rightC, upA, upB, upC);

	//now normalize all of them


	lenght = sqrt(rightA * rightA + rightB * rightB + rightC * rightC);
	rightA = rightA / lenght;
	rightB = rightB / lenght;
	rightC = rightC / lenght;


	lenght = sqrt(upA * upA + upB * upB + upC * upC);
	upA = upA / lenght;
	upB = upB / lenght;
	upC = upC / lenght;
	/*
	 * look at should be looking like:
	 * R R R 0
	 * U U U 0
	 * D D D 0
	 * 0 0 0 1
	 */
	lookAtMatrix[0] = rightA;
	lookAtMatrix[1] = rightB;
	lookAtMatrix[2] = rightC;

	lookAtMatrix[4] = -1 *upA;
	lookAtMatrix[5] = -1 *upB;
	lookAtMatrix[6] = -1 *upC;

	lookAtMatrix[8] = lookAtX;
	lookAtMatrix[9] = lookAtY;
	lookAtMatrix[10] = lookAtZ;
	//std::cout << "mouse move calculated" << std::endl;
	std::cout << lookAtMatrix[0] << " " << lookAtMatrix[1] << " " << lookAtMatrix[2] << " " << lookAtMatrix[3] << std::endl;
	std::cout << lookAtMatrix[4] << " " << lookAtMatrix[5] << " " << lookAtMatrix[6] << " " << lookAtMatrix[7] << std::endl;
	std::cout << lookAtMatrix[8] << " " << lookAtMatrix[9] << " " << lookAtMatrix[10] << " " << lookAtMatrix[11] << std::endl;
	std::cout << lookAtMatrix[12] << " " << lookAtMatrix[13] << " " << lookAtMatrix[14] << " " << lookAtMatrix[15] << std::endl;
	std::cout << std::endl;
}

void GlHelper::reshape (int w, int h)
{
    perspectiveMatrix[0] = 2.4f / (w / (float)h);
    perspectiveMatrix[5] = 2.4f;
    
    glUseProgram(theProgram);
    glUniformMatrix4fv(perspectiveMatrixM, 1, GL_FALSE, perspectiveMatrix);
    glUseProgram(0);
    this->width = width;
    this->height = height;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}
