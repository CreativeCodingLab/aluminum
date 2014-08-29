

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

attribute vec4 vertexPosition;
attribute vec3 vertexTexCoord;

varying vec2 texCoord;

void main() {  
  texCoord = vertexTexCoord.xy;
  gl_Position = proj * view * model * vertexPosition; 
} 

