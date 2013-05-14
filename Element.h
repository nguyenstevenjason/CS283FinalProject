/* 
 * File:   Element.h
 * Author: tomma_000
 *
 * Created on April 7, 2013, 4:44 PM
 */

#ifndef ELEMENT_H
#define	ELEMENT_H
#include <string>
#include <vector>
#include "Joint.h"
using namespace std;
class Joint;

class Element
{
public:
	Element();
	virtual ~Element();
        string thisType;
        string shapeType;
        glm::vec3 GlobalRot;
        glm::vec3 trans;
        glm::vec3 scale;
        Joint* parent;
        Joint* child;
        glm::vec3 getPos();
        glm::vec3 getTotRot();
        glm::vec3 getBegin();
        glm::vec3 getEnd();
        float len;
        vector< Joint* > rootChildren;
};


#endif	/* ELEMENT_H */

