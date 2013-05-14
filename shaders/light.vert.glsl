# version 120 


// Mine is an old machine.  For version 130 or higher, do 
// out vec4 color ;  
// out vec4 mynormal ; 
// out vec4 myvertex ;
// That is certainly more modern

varying vec4 color ; 
varying vec3 mynormal ; 
varying vec4 myvertex ; 

uniform int isWarp;

void main() {
	vec4 temp = vec4(gl_Vertex);
	if (isWarp == 1) {
		temp.z = 0.1 * sin(20 * temp.x);
		temp.x = 0.15 * sin(10 * temp.y);
	}
    gl_TexCoord[0] = gl_MultiTexCoord0 ;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * temp; 
	color = gl_Color; 
    mynormal = gl_Normal ; 
    myvertex = gl_Vertex ;  
}

