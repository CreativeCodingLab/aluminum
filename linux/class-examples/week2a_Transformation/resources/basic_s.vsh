#version 150 

uniform mat4 p;
uniform mat4 m;
uniform mat4 v;

in vec4 vertexPosition; 
in vec4 vertexColor; 


out vec4 color;

void main() {  
  color = vertexColor; 
  gl_Position = p * v * m * vertexPosition;
} 

