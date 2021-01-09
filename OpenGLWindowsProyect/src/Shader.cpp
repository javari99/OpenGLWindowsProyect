//
//  Shader.cpp
//  OpenGLProject
//
//  Created by Patxi Arin Reiriz on 8/5/20.
//  Copyright © 2020 Javier Arin Reiriz. All rights reserved.
//

#include "Shader.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "JavUtils.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    
    //Read the files
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    //Try read the vertex shader
    try {
        vShaderFile.open(vertexPath);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();
    } catch (std::ifstream::failure e) {
        JUtils::printError("SHADER::Could_not_read_vertexShader");
        
    }
    const char* vShaderCode = vertexCode.c_str();
    
    //Try read the fragment shader
    try {
        fShaderFile.open(fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        JUtils::printError("SHADER::Could_not_read_vertexShader");
    }
    const char* fShaderCode = fragmentCode.c_str();
    
    //Compile and link shaders
    
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    //Check compile errors
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        JUtils::printError("SHADER::VERTEX::shader_compilation_failed: ");
        JUtils::printError(infoLog);
    }
    
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    //Check compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        JUtils::printError("SHADER::FRAGMENT::shader_compilation_failed: ");
        JUtils::printError(infoLog);
    }
    
    //Link the shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    //Check link errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        JUtils::printError("SHADER::PROGRAM::shader_link_failed ");
        JUtils::printError(infoLog);
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    //Finished creating shader
}

Shader::~Shader() { 
    glDeleteProgram(ID);
}

void Shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setBool(const std::string &name, bool value) const { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setMat4(const std::string &name, glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string &name, glm::mat3& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE, &value[0][0]);
}

void Shader::setMat2(const std::string &name, glm::mat2& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE, &value[0][0]);;
}

void Shader::setVec4(const std::string &name, glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, glm::vec3& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);;
}

void Shader::setVec2(const std::string &name, glm::vec2& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);;
}

unsigned int Shader::getID() { 
    return ID;
}

void Shader::use() { 
    glUseProgram(ID);
}





