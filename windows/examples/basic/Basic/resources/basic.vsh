#version 150 

uniform mat4 proj;
uniform mat4 mv;
in vec4 vertexPosition; 
in vec4 vertexColor; 
out vec4 color; 

void main() {  
  color = vertexColor; 
  gl_Position = proj * mv * vertexPosition; 
} 

