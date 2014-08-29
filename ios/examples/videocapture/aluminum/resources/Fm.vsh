uniform mat4 proj;
uniform mat4 mv;
uniform vec2 MouseCords;

attribute vec4 vertexPosition; 
attribute vec3 vertexTexCoord;

varying vec2 texCoord;

//varying vec4 color;

void main() {
  //  vec4 outColor;
    texCoord = vertexTexCoord.xy;
   
   // Grayval = cos(20.0*(1.0-Grayval));
//    Grayval = clamp(Grayval,0.0,1.0);
//    color = vec4(Grayval,Grayval,Grayval,1.0);
    gl_Position = proj * mv * vertexPosition;
} 

