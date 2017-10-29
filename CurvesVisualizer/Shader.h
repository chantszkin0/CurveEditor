//
//  Shader.h
//  CurvesVisualizer
//
//  Created by Chan Tsz Kin on 25/10/2017.
//  Copyright © 2017 Chan Tsz Kin. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

static const char* vShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";
static const char* fShaderSource = "#version 330 core\n"
"uniform bool isPolyline;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   if(isPolyline)\n"
"       FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"   else\n"
"       FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
"}\n\0";

/*
 *
 *  Compile Shader Codes
 *
 */
unsigned int loadShader(){
    // vertex Shader
    unsigned int vertex, fragment, ID;
    int success;
    char infoLog[512];
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderSource, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER.CPP::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderSource, NULL);
    glCompileShader(fragment);
    // check for shader compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER.CPP::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    /*
     *
     *  Create Shader Program and Linking
     *
     */
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER.CPP::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return ID;
}

#endif /* Shader_h */
