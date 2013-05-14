// Transform.cpp: implementation of the Transform class.

#include "Transform.h"
#include "math.h"

//Takes as input the current eye position, and the current up vector.
//up is always normalized to a length of 1.
//eye has a length indicating the distance from the viewer to the origin

// Helper rotation function.  Please implement this.  

mat3 Transform::rotate(const float degrees, const vec3& axis) {
  mat3 R ; 
  float pideg = 180;
  float pi = 4.0 * atan(1.0);
  mat3 I = mat3(1.0, 0, 0, 0, 1.0, 0, 0, 0, 1.0);
  float deg = degrees * pi / pideg;
  float sinz = sin(deg);
  float cosz = cos(deg);

  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  vec3 a = vec3(0, z, -y);
  vec3 b = vec3(-z, 0, x);
  vec3 c = vec3(y, -x, 0);
  mat3 crosmat = mat3(a, b, c);

  vec3 ta = vec3(x*x, x*y, x*z);
  vec3 tb = vec3(x*y, y*y, y*z);
  vec3 tc = vec3(x*z, y*z, z*z);
  mat3 tensmat = mat3(ta, tb, tc);

  R = I*cosz + sinz*crosmat + (1 - cosz)*tensmat;  
  return R ; 
}

void Transform::left(float degrees, vec3& eye, vec3& up) {
	mat3 rotmat = Transform::rotate(degrees, up);
	eye = rotmat * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
	vec3 rotvect = glm::normalize(glm::cross(eye, up));
	vec3 nup = glm::normalize(glm::cross(rotvect, eye));
	mat3 rotmat = Transform::rotate(degrees, rotvect);
	eye = rotmat * eye;
	up = nup;
}

mat4 Transform::lookAt(vec3 eye, vec3 up) {
    mat4 M ; 
	vec3 origin = vec3(0.0, 0.0, 0.0);
	vec3 newz = glm::normalize(eye);
	vec3 newx = glm::normalize(glm::cross(up, eye));
	vec3 newy = glm::normalize(glm::cross(eye, newx));
	float transx = - glm::dot(newx, eye);
	float transy = - glm::dot(newy, eye);
	float transz = - glm::dot(newz, eye);
	vec3 trans = vec3(transx, transy, transz);
	M = mat4(newx.x, newx.y, newx.z, trans.x,
		     newy.x, newy.y, newy.z, trans.y,
			 newz.x, newz.y, newz.z, trans.z,
			 0, 0, 0, 1.0);
	// the camera starts out in the negative z "view" aka points to (0,0,-1)
	return M ; 
}

Transform::Transform()
{

}

Transform::~Transform()
{

}

// Some notes about using glm functions.
// You are ONLY permitted to use glm::dot glm::cross glm::normalize
// Do not use more advanced glm functions (in particular, directly using 
// glm::lookAt is of course prohibited).  

// You may use overloaded operators for matrix-vector multiplication 
// But BEWARE confusion between opengl (column major) and row major 
// conventions, as well as what glm implements. 
// In particular, vecnew = matrix * vecold may not implement what you think 
// it does.  It treats matrix as column-major, in essence using the transpose.
// We recommend using row-major and vecnew = vecold * matrix 
// Preferrably avoid matrix-matrix multiplication altogether for this hw.  

// http://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions