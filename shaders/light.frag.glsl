# version 120 

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color ;  
// in vec4 mynormal ; 
// in vec4 myvertex ;
// That is certainly more modern

varying vec4 color ;
varying vec3 mynormal ; 
varying vec4 myvertex ; 

uniform sampler2D tex ; 
uniform int istex ; 
uniform int islight ; // are we lighting. 
uniform float isBlur;

uniform vec3 light0dirn ; 
uniform vec4 light0posn ;
uniform vec4 light0color ; 
uniform vec4 light1posn ; 
uniform vec4 light1color ; 

uniform vec4 ambient ; 
uniform vec4 diffuse ; 
uniform vec4 specular ; 
uniform float shininess ;
uniform vec4 emission ;	 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        float nDotL = dot(normal, direction)  ;         
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 

        vec4 retval = lambert + phong ; 
        return retval ;            
}       

void main (void) 
{       

	// WITH ALOT OF HELP FROM http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/
    float blurSize = isBlur/512.0;
    if ((istex > 0) && (isBlur > 0)) { 
		gl_FragColor = texture2D(tex, vec2(gl_TexCoord[0].s - 4 *blurSize, gl_TexCoord[0].t- 4 *blurSize)) * 0.05 ;
		gl_FragColor += texture2D(tex, vec2(gl_TexCoord[0].s - 3 *blurSize, gl_TexCoord[0].t- 3 *blurSize)) * 0.09 ; 
		gl_FragColor += texture2D(tex, vec2(gl_TexCoord[0].s - 2 *blurSize, gl_TexCoord[0].t- 2 *blurSize)) * 0.12 ; 
		gl_FragColor += texture2D(tex, vec2(gl_TexCoord[0].s - 1 *blurSize, gl_TexCoord[0].t- 1 *blurSize)) * 0.15 ;
		gl_FragColor += texture2D(tex, gl_TexCoord[0].st) * 0.16;
		gl_FragColor += texture2D(tex, vec2(gl_TexCoord[0].s + 1 *blurSize, gl_TexCoord[0].t+ 1 *blurSize)) * 0.15 ;
		gl_FragColor += texture2D(tex, vec2(gl_TexCoord[0].s + 2 *blurSize, gl_TexCoord[0].t+ 2 *blurSize)) * 0.12 ; 
		gl_FragColor += texture2D(tex, vec2(gl_TexCoord[0].s + 3 *blurSize, gl_TexCoord[0].t+ 3 *blurSize)) * 0.09 ; 
		gl_FragColor += texture2D(tex, vec2(gl_TexCoord[0].s + 4 *blurSize, gl_TexCoord[0].t+ 4 *blurSize)) * 0.05 ; 
		}
	else if (istex > 0) gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
    
    else if (islight == 0) gl_FragColor = color ; 
    
    else { 
        // They eye is always at (0,0,0) looking down -z axis 
        // Also compute current fragment position and direction to eye 

        const vec3 eyepos = vec3(0,0,0) ; 
        vec4 _mypos = gl_ModelViewMatrix * myvertex ; 
        vec3 mypos = _mypos.xyz / _mypos.w ; // Dehomogenize current location 
        vec3 eyedirn = normalize(eyepos - mypos) ; 

        // Compute normal, needed for shading. 
        // Simpler is vec3 normal = normalize(gl_NormalMatrix * mynormal) ; 
        vec3 _normal = (gl_ModelViewMatrixInverseTranspose*vec4(mynormal,0.0)).xyz ; 
        vec3 normal = normalize(_normal) ; 

        // Light 0, directional
        //vec3 direction0 = normalize (light0dirn) ; 
        //vec3 half0 = normalize (direction0 + eyedirn) ; 
        //vec4 col0 = ComputeLight(direction0, light0color, normal, half0, diffuse, specular, shininess) ;
        vec3 position0 = light0posn.xyz / light0posn.w ; 
        vec3 direction0 = normalize (position0 - mypos) ; // no attenuation 
        vec3 half0 = normalize (direction0 + eyedirn) ;  
        vec4 col0 = ComputeLight(direction0, light0color, normal, half0, diffuse, specular, shininess) ;

        // Light 1, point 
        vec3 position1 = light1posn.xyz / light1posn.w ; 
        vec3 direction1 = normalize (position1 - mypos) ; // no attenuation 
        vec3 half1 = normalize (direction1 + eyedirn) ;  
        vec4 col1 = ComputeLight(direction1, light1color, normal, half1, diffuse, specular, shininess) ;
        
        gl_FragColor = ambient + emission + col0 + col1;
        }
}
