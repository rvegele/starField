//#version 120
// ORIGINAL
//uniform float starSize;

//void main() {
    
//	vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
    
//    gl_Position = gl_ProjectionMatrix * eyeCoord;
    
//	float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
//	float att	 = 600.0 / dist;
    
//	gl_PointSize = starSize * att - 1.5;
//	gl_FrontColor = gl_Color;
//}

// NEW
varying vec2 texcoord;

uniform float starSize;

varying vec4 vVertex;
varying vec4 vColor;

void main() {
    
    vVertex = gl_Vertex; // vertex position
    //vColor = gl_Color;
    
    vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex; // where's the eye
    
    texcoord = vec2 (gl_TextureMatrix[0] * gl_MultiTexCoord0);
    
    //gl_TexCoord[0] = gl_MultiTexCoord0; // do stuff with textures
    
    float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z); // distance to eye
    float att	 = 600.0 / dist;
    
    //nuo 0 iki 128 * att; nuo 0 iki att
    gl_PointSize = starSize * (150.0) * att;
    //gl_PointSize = starSize * 128.0 * att;// + 64.0;// att;// * x * 2.0;
    
    gl_Position = gl_ProjectionMatrix * eyeCoord;
    //gl_Position = ftransform();
    
    gl_FrontColor = gl_Color;
}