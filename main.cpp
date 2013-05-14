/*****************************************************************************/
/* This is the program skeleton for homework 1 in CS 184 by Ravi Ramamoorthi */
/* Based on an assignment designed by former TA Aner Ben-Artzi               */
/* Rewritten to be modern in terms of shaders etc.                           */
/* The idea is to draw a teapot, that can be moved by a crystal ball         */
/* interface                                                                 */
/*****************************************************************************/

#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "shaders.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Joint.h"
#include "Element.h"

int amount; // The amount of rotation for each arrow press

glm::vec4 change;
glm::vec3 eye; // The (regularly updated) vector coordinates of the eye location 
glm::vec3 up;  // The (regularly updated) vector coordinates of the up location 
const glm::vec3 eyeinit(0.0,0.0,5.0) ; // Initial eye position, also for resets
const glm::vec3 upinit(0.0,1.0,0.0) ; // Initial up position, also for resets
bool useGlu; // Toggle use of "official" opengl/glm transform vs user code
int w, h; // width and height 
GLuint vertexshader, fragmentshader, shaderprogram ; // shaders

// Constants to set up lighting on the teapot
const GLfloat light_position[] = {0, 5, 5, 1};    // Position of light 0
const GLfloat light_position1[] = {0, 5, -5, 1};  // Position of light 1
const GLfloat light_specular[] = {0.9, 0.0, 0, 1};    // Specular of light 0
const GLfloat light_specular1[] = {0, 0.0, 0.9, 1};   // Specular of light 1
const GLfloat one[] = {1, 1, 1, 1};                 // Specular on teapot
const GLfloat medium[] = {0.2, 0.2, 0.2, 1};        // Diffuse on teapot
const GLfloat small[] = {0.2, 0.2, 0.2, 1};         // Ambient on teapot
const GLfloat high[] = {100} ;                      // Shininess of teapot
GLfloat light0[4], light1[4], temp[3] ; 

// Variables to set uniform params for lighting fragment shader 
GLuint islight ; 
GLuint light0posn ; 
GLuint light0color ; 
GLuint light1posn ; 
GLuint light1color ; 
GLuint ambient ; 
GLuint diffuse ; 
GLuint specular ; 
GLuint shininess ; 
GLuint moo;
GLuint isSin;

Element* root;
Element* reachObj;
vector<Element*> allElements;
vector<Joint*> allJoints;

/* FUNCTIONS */
void draw(Joint* temp);
void draw(Element* temp);
void inverseK(Joint* temp);
void inverseK(Element* temp);

/* Keyboard Functionality*/
int isRotate = 1;
int isTranslate = 0;
bool isS = false;

float transx = 0;
float transy = 0;
float transz = 0;

float rotx = 0;
float roty = 0;

/* MOUSE FUNCTIONALITY*/
int mouseoldx = 0;
int mouseoldy = 0;
void createCylinder() {
	GLUquadric* qobj;
	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glPushMatrix();
                glRotatef(90, 0.0, 1.0, 0.0);
                glScalef(7.0, 7.0, 1.0);
		gluCylinder(qobj, 0.05, 0.05, 0.5, 20, 20);
	glPopMatrix();
}
void createSphere(int x, int y) {
    float tempx = 10.0f * (float) (x - 400.0f) / 400.0f;
    float tempy = -10.0f * (float) (y - 400.0f) / 400.0f;
    /*GLfloat projMat[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projMat);
    glm::vec4 mousepos = glm::vec4(x, y, 0, 1);
    glm::mat4 projMatrix = glm::mat4(projMat[0], projMat[1], projMat[2], projMat[3],
                                        projMat[4], projMat[5], projMat[6], projMat[7],
                                        projMat[8], projMat[9], projMat[10], projMat[11],
                                        projMat[12], projMat[13], projMat[14], projMat[15]
            );
    glm::vec4 newPos = glm::inverse(glm::transpose(projMatrix)) * mousepos;*/
    reachObj = new Element();
    reachObj->shapeType = "sphere";
    /*reachObj->trans = glm::vec3(newPos.x, newPos.y, newPos.z);
    cout << newPos.x << endl;
    cout << newPos.y << endl;
    cout << newPos.z << endl;
    cout << newPos.w << endl;*/
    reachObj->trans = glm::vec3(tempx, tempy, 0);
}
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            reachObj = NULL;
        }
        else if (state == GLUT_DOWN) {
            mouseoldx = x; mouseoldy = y;
            createSphere(x, y);
        }
  }
  if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_UP) {
            reachObj = NULL;
	}
	else if (state == GLUT_DOWN) {
	    mouseoldx = x; mouseoldy = y;
            reachObj = NULL;
	}
  }
  glutPostRedisplay();
}
void mousedrag(int x, int y) {
    if (reachObj != NULL) {
	mouseoldx = x; mouseoldy = y;
        createSphere(x, y);
    }
    glutPostRedisplay();
}
void transformvec (const GLfloat input[4], GLfloat output[4]) {
  GLfloat modelview[16] ; // in column major order
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview) ; 
  
  for (int i = 0 ; i < 4 ; i++) {
    output[i] = 0 ; 
    for (int j = 0 ; j < 4 ; j++) 
      output[i] += modelview[4*j+i] * input[j] ; 
  }
}
void keyboard(unsigned char key, int x, int y) {
	switch(key) {
            case 27:  // Escape to quit
		exit(0) ;
                break;
            case 'r':
                isRotate = 1;
                isTranslate = 0;
                break;
            case 't':
                isTranslate = 1;
                isRotate = 0;
                break;
            case 's':
                isS = !isS;
                break;
        }
	glutPostRedisplay();
}
void specialKey(int key, int x, int y) {
	switch(key) {
	case 100: //left
            if (isRotate == 1) {
                rotx -= 2.0;
            } else if (isTranslate == 1) {
                transx -= 0.1;
            }
		break;
	case 101: //up
            if (isRotate == 1) {
                roty -= 2.0;
            } else if (isTranslate == 1) {
                transy += 0.1;
            }
		break;
	case 102: //right
            if (isRotate == 1) {
                rotx += 2.0;
            } else if (isTranslate == 1) {
                transx += 0.1;
            }
		break;
	case 103: //down
            if (isRotate == 1) {
                roty += 2.0;
            } else if (isTranslate == 1) {
                transy -= 0.1;
            }
		break;
	}
	glutPostRedisplay();
}
void reshape(int width, int height){
	w = width;
	h = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, w/(float)h, 0.1, 1000.0);
	glViewport(0, 0, w, h);
}
void initShading() {
      eye = glm::vec3(0.0, 0.0, 5.0);   
      up = glm::vec3(0.0, 1.0, 0.0);
      amount = 5;

      glEnable(GL_DEPTH_TEST);

      vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
      fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
      shaderprogram = initprogram(vertexshader, fragmentshader) ; 
      islight = glGetUniformLocation(shaderprogram,"islight") ;        
      light0posn = glGetUniformLocation(shaderprogram,"light0posn") ;       
      light0color = glGetUniformLocation(shaderprogram,"light0color") ;       
      light1posn = glGetUniformLocation(shaderprogram,"light1posn") ;       
      light1color = glGetUniformLocation(shaderprogram,"light1color") ;       
      ambient = glGetUniformLocation(shaderprogram,"ambient") ;       
      diffuse = glGetUniformLocation(shaderprogram,"diffuse") ;       
      specular = glGetUniformLocation(shaderprogram,"specular") ;       
      shininess = glGetUniformLocation(shaderprogram,"shininess") ;
      isSin = glGetUniformLocation(shaderprogram, "isSin");
}
void initShapes() {
    root = new Element();
    root->parent = NULL;
    root->shapeType = "cube";
    root->scale = glm::vec3(2.0, 2.0, 2.0);
    allElements.push_back(root);
}
void createTwoTentacles(){
    Joint* newJoint = new Joint();
    newJoint->parent = root;
    newJoint->shapeType = "sphere";
    newJoint->trans = glm::vec3(1.0, 0, 0);
    newJoint->scale = glm::vec3(0.5, 0.5, 0.5);
    //newJoint->rot = glm::vec3(0.0, 90.0, 0.0);
    root->rootChildren.push_back(newJoint);
    allJoints.push_back(newJoint);
    
    Element* newLimb = new Element();
    newLimb->parent = newJoint;
    newLimb->shapeType = "cylinder";
    newLimb->trans = glm::vec3(0, 0, 0);
    newLimb->scale = glm::vec3(4.0, 1.0, 1.0);
    newLimb->len = 2.0;
    newJoint->child = newLimb;
    allElements.push_back(newLimb);
    
    Joint* newJoint2 = new Joint();
    newJoint2->parent = newLimb;
    newJoint2->shapeType = "sphere";
    newJoint2->trans = glm::vec3(2.0, 0, 0);
    newJoint2->scale = glm::vec3(0.5, 0.5, 0.5);
    newJoint2->rotAxis = glm::vec3(0.0, 0.0, 1.0);
    newLimb->child = newJoint2;
    allJoints.push_back(newJoint2);
    
    Element* newLimb2 = new Element();
    newLimb2->parent = newJoint2;
    newLimb2->shapeType = "cylinder";
    newLimb2->trans = glm::vec3(0, 0, 0);
    newLimb2->scale = glm::vec3(4.0, 1.0, 1.0);
    newLimb2->len = 2.0f;
    newJoint2->child = newLimb2;
    allElements.push_back(newLimb2);
    
    /*Joint* newJoint3 = new Joint();
    newJoint3->parent = newLimb2;
    newJoint3->shapeType = "sphere";
    newJoint3->trans = glm::vec3(2.0, 0, 0);
    newJoint3->scale = glm::vec3(0.5, 0.5, 0.5);
    newLimb2->child = newJoint3;
    allJoints.push_back(newJoint3);
    
    Element* newLimb3 = new Element();
    newLimb3->parent = newJoint3;
    newLimb3->shapeType = "cylinder";
    newLimb3->trans = glm::vec3(0, 0, 0);
    newLimb3->scale = glm::vec3(4.0, 1.0, 1.0);
    newLimb3->len = 2.0f;
    newJoint3->child = newLimb3;
    allElements.push_back(newLimb3);*/
}
void createTwoTentaclesv2(){
    Joint* newJoint = new Joint();
    newJoint->parent = root;
    newJoint->shapeType = "sphere";
    newJoint->trans = glm::vec3(1.0, 0, 0);
    newJoint->scale = glm::vec3(0.5, 0.5, 0.5);
    //newJoint->rot = glm::vec3(0.0, 90.0, 0.0);
    root->rootChildren.push_back(newJoint);
    allJoints.push_back(newJoint);
    
    Element* newLimb = new Element();
    newLimb->parent = newJoint;
    newLimb->shapeType = "cylinder";
    newLimb->trans = glm::vec3(0, 0, 0);
    newLimb->scale = glm::vec3(8.0, 1.0, 1.0);
    newLimb->len = 4.0;
    newJoint->child = newLimb;
    allElements.push_back(newLimb);
    
    Joint* newJoint2 = new Joint();
    newJoint2->parent = newLimb;
    newJoint2->shapeType = "sphere";
    newJoint2->trans = glm::vec3(4.0, 0, 0);
    newJoint2->scale = glm::vec3(0.5, 0.5, 0.5);
    newJoint2->rotAxis = glm::vec3(0.0, 0.0, 1.0);
    newLimb->child = newJoint2;
    allJoints.push_back(newJoint2);
    
    Element* newLimb2 = new Element();
    newLimb2->parent = newJoint2;
    newLimb2->shapeType = "cylinder";
    newLimb2->trans = glm::vec3(0, 0, 0);
    newLimb2->scale = glm::vec3(8.0, 1.0, 1.0);
    newLimb2->len = 4.0f;
    newJoint2->child = newLimb2;
    allElements.push_back(newLimb2);
    
    /*Joint* newJoint3 = new Joint();
    newJoint3->parent = newLimb2;
    newJoint3->shapeType = "sphere";
    newJoint3->trans = glm::vec3(2.0, 0, 0);
    newJoint3->scale = glm::vec3(0.5, 0.5, 0.5);
    newLimb2->child = newJoint3;
    allJoints.push_back(newJoint3);
    
    Element* newLimb3 = new Element();
    newLimb3->parent = newJoint3;
    newLimb3->shapeType = "cylinder";
    newLimb3->trans = glm::vec3(0, 0, 0);
    newLimb3->scale = glm::vec3(4.0, 1.0, 1.0);
    newLimb3->len = 2.0f;
    newJoint3->child = newLimb3;
    allElements.push_back(newLimb3);*/
}
void createNTentacles(int n, glm::vec3 trans, glm::vec3 rot, float len){
    Joint* newJoint = NULL;
    Element* newLimb = root;
    
    newJoint = new Joint();
    newJoint->parent = newLimb;
    newJoint->shapeType = "sphere";
    newJoint->trans = trans;
    newJoint->scale = glm::vec3(0.5, 0.5, 0.5);
    newJoint->rot = rot;
    root->rootChildren.push_back(newJoint);
    allJoints.push_back(newJoint);

    newLimb = new Element();
    newLimb->parent = newJoint;
    newLimb->shapeType = "cylinder";
    newLimb->trans = glm::vec3(0, 0, 0);
    newLimb->scale = glm::vec3((2*len), 1.0, 1.0);
    newLimb->len = len;
    newJoint->child = newLimb;
    allElements.push_back(newLimb);
    
    for (int i = 0; i < n; i++) {
        newJoint = new Joint();
        newJoint->parent = newLimb;
        newJoint->shapeType = "sphere";
        newJoint->trans = glm::vec3(len, 0, 0);
        newJoint->scale = glm::vec3(0.5, 0.5, 0.5);
        newJoint->rotAxis = glm::vec3(0.0, 0.0, 1.0);
        newLimb->child = newJoint;
        allJoints.push_back(newJoint);

        newLimb = new Element();
        newLimb->parent = newJoint;
        newLimb->shapeType = "cylinder";
        newLimb->trans = glm::vec3(0, 0, 0);
        newLimb->scale = glm::vec3((2 * len), 1.0, 1.0);
        newLimb->len = len;
        newJoint->child = newLimb;
        allElements.push_back(newLimb);
    }
}
void draw(Element* temp) {
     glPushMatrix();
     glTranslatef(temp->trans.x, temp->trans.y, temp->trans.z);
     
     if (temp == root) {
        glRotatef(temp->GlobalRot.x, 1.0, 0.0, 0.0);
        glRotatef(temp->GlobalRot.y, 0.0, 1.0, 0.0);
        glRotatef(temp->GlobalRot.z, 0.0, 0.0, 1.0);
     }
     
     glPushMatrix();
     glScalef(temp->scale.x, temp->scale.y, temp->scale.z);
     if (temp->shapeType.compare("cube") == 0) {
        glutSolidCube(1.0);
     }
     if (temp->shapeType.compare("sphere") == 0) {
        glutSolidSphere(1.0, 100, 100);
     }
     if (temp->shapeType.compare("cone") == 0) {
         glPushMatrix();
         glRotatef(-90, 0.0, 1.0, 0.0);
         glutSolidCone(1.0,1.0, 100, 100);
         glPopMatrix();
     }
     if (temp->shapeType.compare("cylinder") == 0) {
         createCylinder();
     }
     glPopMatrix();
     
     if ((temp->child != NULL) && (temp != root)) {
         draw(temp->child);
     }
     if (temp == root) {
         for (int i = 0; i < root->rootChildren.size(); i++) {
             draw(root->rootChildren[i]);
         }
     }
     glPopMatrix();
}
void draw(Joint* temp) {
     glPushMatrix();
     
     glTranslatef(temp->trans.x, temp->trans.y, temp->trans.z);
     if (temp->rotAxis.x == 1.0) {
        glRotatef(temp->rot.x, 1.0, 0.0, 0.0);
     }
     if (temp->rotAxis.y == 1.0) {
        glRotatef(temp->rot.y, 0.0, 1.0, 0.0);
     }
     if (temp->rotAxis.z == 1.0) {
        glRotatef(temp->rot.z, 0.0, 0.0, 1.0);
     }
     
     glPushMatrix();
     glScalef(temp->scale.x, temp->scale.y, temp->scale.z);
     if (temp->shapeType.compare("cube") == 0) {
        glutSolidCube(1.0);
     }
     if (temp->shapeType.compare("sphere") == 0) {
        glutSolidSphere(1.0, 100, 100);
     }
     if (temp->shapeType.compare("cone") == 0) {
         glPushMatrix();
         glRotatef(-90, 0.0, 1.0, 0.0);
         glutSolidCone(1.0,1.0, 100, 100);
         glPopMatrix();
     }
     if (temp->shapeType.compare("cylinder") == 0) {
         createCylinder();
     }
     glPopMatrix();
     
     
     if (temp->child != NULL) {
         draw(temp->child);
     }
     
     glPopMatrix();
}
float computeError(Element* temp, Element* reachobj) {
    //glm::vec3 reacher = temp->getEnd();
    //glm::vec3 reached = reachobj->trans;
    float error = 0;
    /*error += pow((reacher.x - reached.x), 2);
    error += pow((reacher.y - reached.y), 2);
    error += pow((reacher.z - reached.z), 2);
    error = sqrt(error);*/
    
    if (temp->child != NULL) {
        float nextError = computeError(temp->child->child, reachobj);
        //if (nextError > error) {
            error = nextError;
        //}
    } else {
            glm::vec3 reacher = temp->getEnd();
        glm::vec3 reached = reachobj->trans;
        error += pow((reacher.x - reached.x), 2);
        error += pow((reacher.y - reached.y), 2);
        error += pow((reacher.z - reached.z), 2);
        error = sqrt(error);
    }
    return error;
}
void minimizeError(Element* temp, float error) {
    float newError = error;
    float tempError = 0;
    float changez = 0;
    float changey = 0;
    float newError2 = error;
    float tempError2 = 0;
    float changez2 = 0;
    float changey2 = 0;
    int poop = 0;
    
    //ONE WAY OF DOING THIS
    //CHANGE Y HERE
    if (temp->parent->rotAxis.y != 0) {
        while (true) {
            temp->parent->rot = temp->parent->rot + glm::vec3(0, 0.1, 0);
            changey += 0.1;
            poop += 1;
            tempError = computeError(temp, reachObj);
            if (tempError >= newError) {
                temp->parent->rot = temp->parent->rot - glm::vec3(0, 0.1, 0);
                poop -= 1;
                changey -= 0.1;
                break;
            } else {
                newError = tempError;
            }
        }

        while (poop == 0) {
            temp->parent->rot = temp->parent->rot - glm::vec3(0, 0.1, 0);
            changey -= 0.1;

            tempError = computeError(temp, reachObj);
            if (tempError >= newError) {
                temp->parent->rot = temp->parent->rot + glm::vec3(0, 0.1, 0);
                changey += 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
    }
    poop = 0;
    //CHANGE Z HERE
    if (temp->parent->rotAxis.z != 0) {
        while (true) {
            temp->parent->rot = temp->parent->rot + glm::vec3(0, 0, 0.1);
            changez += 0.1;
            poop += 1;
            tempError = computeError(temp, reachObj);
            if (tempError >= newError) {
                temp->parent->rot = temp->parent->rot - glm::vec3(0, 0, 0.1);
                poop -= 1;
                changez -= 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
        while (poop == 0) {
            temp->parent->rot = temp->parent->rot - glm::vec3(0, 0, 0.1);
            changez -= 0.1;
            tempError = computeError(temp, reachObj);
            if (tempError >= newError) {
                temp->parent->rot = temp->parent->rot + glm::vec3(0, 0, 0.1);
                changez += 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
    }

    //UNDO SHIT HERE
    temp->parent->rot = temp->parent->rot + glm::vec3(0, -changey, -changez);
    poop = 0;
    //OTHER WAY OF DOING THIS
    //CHANGE Z HERE
    if (temp->parent->rotAxis.z != 0) {
        while (true) {
            temp->parent->rot = temp->parent->rot + glm::vec3(0, 0, 0.1);
            changez2 += 0.1;
            poop += 1;
            tempError2 = computeError(temp, reachObj);
            if (tempError2 >= newError2) {
                temp->parent->rot = temp->parent->rot - glm::vec3(0, 0, 0.1);
                poop -= 1;
                changez2 -= 0.1;
                break;
            } else {
                newError2 = tempError2;
            }
        }
        while (poop == 0) {
            temp->parent->rot = temp->parent->rot - glm::vec3(0, 0, 0.1);
            changez2 -= 0.1;
            tempError2 = computeError(temp, reachObj);
            if (tempError2 >= newError2) {
                temp->parent->rot = temp->parent->rot + glm::vec3(0, 0, 0.1);
                changez2 += 0.1;
                break;
            } else {
                newError2 = tempError2;
            }
        }
    }
    //CHANGE Y HERE
    if (temp->parent->rotAxis.y != 0) {
        while (true) {
            temp->parent->rot = temp->parent->rot + glm::vec3(0, 0.1, 0);
            changey2 += 0.1;
            poop += 1;
            tempError2 = computeError(temp, reachObj);
            if (tempError2 >= newError2) {
                temp->parent->rot = temp->parent->rot - glm::vec3(0, 0.1, 0);
                poop -= 1;
                changey2 -= 0.1;
                break;
            } else {
                newError2 = tempError2;
            }
        }

        while (poop == 0) {
            temp->parent->rot = temp->parent->rot - glm::vec3(0, 0.1, 0);
            changey2 -= 0.1;

            tempError2 = computeError(temp, reachObj);
            if (tempError2 >= newError2) {
                temp->parent->rot = temp->parent->rot + glm::vec3(0, 0.1, 0);
                changey2 += 0.1;
                break;
            } else {
                newError2 = tempError2;
            }
        }
    }
    // UNDO STUFF HERE
    temp->parent->rot = temp->parent->rot + glm::vec3(0, -changey2, -changez2);
    
    if (newError2 < newError) {
        temp->parent->rot = temp->parent->rot + glm::vec3(0, changey2, changez2);
    } else {
        temp->parent->rot = temp->parent->rot + glm::vec3(0, changey, changez);
    }
}
void minimizeTransError(Element* temp, bool isRoot) {
    Element* endElem = temp;
    if (isRoot) {
        int choice = 0;
        float error = INFINITY;
        float kazoo = 0;
        for (int i = 0; i < temp->rootChildren.size(); i++) {
            endElem = temp->rootChildren[i]->child;
            while (endElem->child != NULL) {
                endElem = endElem->child->child;
            }
            kazoo = computeError(endElem, reachObj);
            if (kazoo < error) {
                error = kazoo;
                choice = i;
                endElem = temp;
            }
        }
        endElem = temp->rootChildren[choice]->child;
        while (endElem->child != NULL) {
            endElem = endElem->child->child;
        }
        float tempError = 0;
        float newError = computeError(endElem, reachObj);
        //X Translation
        while (true) {
            temp->trans = temp->trans + glm::vec3(0.1, 0, 0);
            transx += 0.1;
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->trans = temp->trans - glm::vec3(0.1, 0, 0);
                transx -= 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
        while (true) {
            temp->trans = temp->trans - glm::vec3(0.1, 0, 0);
            transx -= 0.1;
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->trans = temp->trans + glm::vec3(0.1, 0, 0);
                transx -= 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
        //Y Translation
        while (true) {
            temp->trans = temp->trans + glm::vec3(0, 0.1, 0);
            transy += 0.1;
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->trans = temp->trans - glm::vec3(0, 0.1, 0);
                transy -= 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
        while (true) {
            temp->trans = temp->trans - glm::vec3(0, 0.1, 0);
            transy -= 0.1;
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->trans = temp->trans + glm::vec3(0, 0.1, 0);
                transy += 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
        //Z Translation
        while (true) {
            temp->trans = temp->trans + glm::vec3(0, 0, 0.1);
            transz += 0.1;
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->trans = temp->trans - glm::vec3(0, 0, 0.1);
                transz -= 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
        while (true) {
            temp->trans = temp->trans - glm::vec3(0, 0, 0.1);
            transz -= 0.1;
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->trans = temp->trans + glm::vec3(0, 0, 0.1);
                transz += 0.1;
                break;
            } else {
                newError = tempError;
            }
        }
    } else {
        endElem = temp;

        while (endElem->child != NULL) {
            endElem = endElem->child->child;
        }
        float tempError = 0;
        float newError = computeError(endElem, reachObj);
        //X Translation
        while (true) {
            temp->parent->trans = temp->parent->trans + glm::vec3(0.1, 0, 0);
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->parent->trans = temp->parent->trans - glm::vec3(0.1, 0, 0);
                break;
            } else {
                newError = tempError;
            }
        }
        while (true) {
            temp->parent->trans = temp->parent->trans - glm::vec3(0.1, 0, 0);
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->parent->trans = temp->parent->trans + glm::vec3(0.1, 0, 0);
                break;
            } else {
                newError = tempError;
            }
        }
        //Y Translation
        while (true) {
            temp->parent->trans = temp->parent->trans + glm::vec3(0, 0.1, 0);
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->parent->trans = temp->parent->trans - glm::vec3(0, 0.1, 0);
                break;
            } else {
                newError = tempError;
            }
        }
        while (true) {
            temp->parent->trans = temp->parent->trans - glm::vec3(0, 0.1, 0);
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->parent->trans = temp->parent->trans + glm::vec3(0, 0.1, 0);
                break;
            } else {
                newError = tempError;
            }
        }
        //Z Translation
        while (true) {
            temp->parent->trans = temp->parent->trans + glm::vec3(0, 0, 0.1);
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->parent->trans = temp->parent->trans - glm::vec3(0, 0, 0.1);
                break;
            } else {
                newError = tempError;
            }
        }
        while (true) {
            temp->parent->trans = temp->parent->trans - glm::vec3(0, 0, 0.1);
            tempError = computeError(endElem, reachObj);
            if (tempError >= newError) {
                temp->parent->trans = temp->parent->trans + glm::vec3(0, 0, 0.1);
                break;
            } else {
                newError = tempError;
            }
        }
    }
}
void inverseK(Joint* temp){
    //FOR EACH CHILD DO THIS
    if ((reachObj != NULL) && (temp->child != NULL)) {
        inverseK(temp->child);
    }
}
void inverseK(Element* temp) {
    //FOR EACH CHILD DO THIS
    float error = 0;
    if ((reachObj != NULL)&& (temp != root)) {
        error = computeError(temp, reachObj);
        minimizeError(temp, error);
    }
    if ((reachObj != NULL) && (temp->child != NULL) && (temp != root)) {
        inverseK(temp->child);
    }
    if (temp == root) {
        if (reachObj != NULL) {
            for (int i = 0; i < root->rootChildren.size(); i++) {
                inverseK(root->rootChildren[i]);
            }
            if (!isS) {
                minimizeTransError(root, true);
            }
        }
    }
}
void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glm::mat4 mv; 
        const glm::vec3 center(0.0,0.0,0.0) ; 

        mv = glm::lookAt(eye,center,up) ; 
        glLoadMatrixf(&mv[0][0]) ;
        
	glUniform1i(isSin, 1);
        transformvec(light_position, light0) ; 
        transformvec(light_position1, light1) ; 
	glUniform4fv(light0posn, 1, light0) ; 
        glUniform4fv(light0color, 1, light_specular) ; 
        glUniform4fv(light1posn, 1, light1) ; 
        glUniform4fv(light1color, 1, light_specular1) ; 
		
        glUniform4fv(ambient,1,small) ; 
        glUniform4fv(diffuse,1,medium) ; 
        glUniform4fv(specular,1,one) ; 
        glUniform1fv(shininess,1,high) ; 
        glUniform1i(islight,true) ;
        
        glScalef(0.5, 0.5, 0.5);
        
        glPushMatrix();
        root->trans = glm::vec3(transx, transy, 0.0);
        root->GlobalRot = glm::vec3(0.0, rotx, roty);
        draw(root);
        glPopMatrix();

        if (reachObj != NULL) {
            glPushMatrix();
            glTranslatef(reachObj->trans.x, reachObj->trans.y, reachObj->trans.z);
            glutSolidSphere(0.5, 100.0, 100.0);
            glPopMatrix();
            inverseK(root);
        } else {
            //cout << "NOT DRAWING SPHERE" << endl;
        }
        
	glutSwapBuffers();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Project 4: Inverse Kinematics");
	   
	GLenum err = glewInit() ; 
	if (GLEW_OK != err) { 
		std::cerr << "Error: " << glewGetString(err) << std::endl; 
	} 

	initShading();
        initShapes();
        createTwoTentacles();
        /*createNTentacles(2, glm::vec3(0, 1, 0), glm::vec3(0, 0, 90), 1);
        createNTentacles(3, glm::vec3(-1, 0, 0), glm::vec3(0, 0, -180), 1);
        createNTentacles(4, glm::vec3(0, -1, 0), glm::vec3(0, 0, -90), 1);
        createNTentacles(5, glm::vec3(0, 0, 1), glm::vec3(0, -90, 0), 1);
        createNTentacles(6, glm::vec3(0, 0, -1), glm::vec3(0, 90, 0), 1);*/
        
	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
        glutMouseFunc(mouse) ;
        glutMotionFunc(mousedrag) ;
	glutReshapeWindow(800, 800);
	glutMainLoop();
	return 0;
}
