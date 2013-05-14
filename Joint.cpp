#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shaders.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Joint.h"
#include "Element.h"

using namespace std;

Joint::Joint()
{
    this->child = NULL;
    this->parent = NULL;
    this->trans = glm::vec3(0.0, 0.0, 0.0);
    this->scale = glm::vec3(1.0, 1.0, 1.0);
    this->shapeType = "";
    this->rot = glm::vec3(0.0, 0.0, 0.0);
    this->rotAxis = glm::vec3(1.0, 1.0, 1.0);
    this->thisType = "ballJoint";
}

Joint::~Joint()
{
    
}
glm::vec3 Joint::getTotRot() {
    return (this->parent->getTotRot() + this->rot);
}

glm::vec3 Joint::getPos(){
    glm::vec3 pos;
    glm::vec3 rotations = this->getTotRot() - this->rot;
    float radX = rotations.x * M_PI / 180.0f;
    float radY = rotations.y * M_PI / 180.0f;
    float radZ = rotations.z * M_PI / 180.0f;
    //glm::vec3 radRot = glm::vec3(radX, radY, radZ);
    
    if (this->parent != NULL) {
        pos = this->parent->getPos();
    }
    if (this->shapeType.compare("sphere") == 0) {
        glm::vec4 oldTrans = glm::vec4(this->trans.x, this->trans.y, this->trans.z, 1.0);
        
        float psi = radX;
        float theta = radY;
        float phi = radZ;
        glm::mat3 rotAllVec = glm::mat3(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta),
            sin(psi) * sin(theta) * cos(phi) - cos(psi) * sin(phi), cos(psi) * cos(phi) + sin(psi) * sin(theta) * sin(phi), sin(psi) * cos(theta),
            sin(psi) * sin(phi) + cos(psi) * sin(theta) * cos(phi), cos(psi) * sin(theta) * sin(phi) - sin(psi) * cos(phi), cos(psi) * cos(theta)
        );
        glm::vec3 Zaxis = glm::vec3(0, 0, 1);
        glm::vec3 Xaxis = glm::vec3(1, 0, 0);
        glm::vec3 Yaxis = glm::vec3(0, 1, 0);
        
        Zaxis = rotAllVec * Zaxis;
        Xaxis = rotAllVec * Xaxis;
        Yaxis = rotAllVec * Yaxis;
        
        glm::quat Xrot = glm::angleAxis(rotations.x, Xaxis);
        glm::quat Yrot = glm::angleAxis(rotations.y, Yaxis);
        glm::quat Zrot = glm::angleAxis(rotations.z, Zaxis);
        
        glm::mat4 XrotM = glm::toMat4(Xrot);
        glm::mat4 YrotM = glm::toMat4(Yrot);
        glm::mat4 ZrotM = glm::toMat4(Zrot);
        
        oldTrans = XrotM * YrotM * ZrotM * oldTrans;
        pos += glm::vec3(oldTrans.x, oldTrans.y, oldTrans.z);
    }
    //NOTE: NEGATIVE Y DIR
    return pos;
}