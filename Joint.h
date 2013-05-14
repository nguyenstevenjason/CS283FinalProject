/* 
 * File:   Joint.h
 * Author: tomma_000
 *
 * Created on April 7, 2013, 4:44 PM
 */

#ifndef JOINT_H
#define	JOINT_H

#include <string>
#include "Element.h"
using namespace std;
class Element;

class Joint 
{
public:
	Joint();
	virtual ~Joint();
        /* ballJoint, pinJoint, prismJoint*/
        string thisType;
        string shapeType;
        /* This only applies of pinJoint and ballJoint*/
        glm::vec3 rotAxis;
        glm::vec3 rot;
        glm::vec3 trans;
        glm::vec3 scale;
        Element* parent;
        Element* child;
        glm::vec3 getPos();
        glm::vec3 getTotRot();
};


#endif	/* JOINT_H */

