//
//  Shader.hpp
//  OpenGLProject
//
//  Created by Patxi Arin Reiriz on 8/5/20.
//  Copyright © 2020 Javier Arin Reiriz. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <glm/glm.hpp>

class Shader{
    
    unsigned int ID;
    
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use();
    unsigned int getID();
    
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, glm::vec2& value) const;
    void setVec3(const std::string& name, glm::vec3& value) const;
    void setVec4(const std::string& name, glm::vec4& value) const;
    void setMat2(const std::string& name, glm::mat2& value) const;
    void setMat3(const std::string& name, glm::mat3& value) const;
    void setMat4(const std::string& name, glm::mat4& value) const;
};

#endif /* Shader_hpp */
