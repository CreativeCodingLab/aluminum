

#version 150 

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

in vec4 vertexPosition; 
in vec4 vertexTexCoord;

smooth out vec2 texCoord;

void main() {  
  texCoord = vertexTexCoord.xy;
  gl_Position = proj * view * model * vertexPosition; 
} 

