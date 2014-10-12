#version 330 core

in vec4 vertexPosition; 
in vec3 vertexTexCoord;

out vec2 texCoord;

void main() {  
  texCoord = vertexTexCoord.xy;
  gl_Position = vertexPosition;
}

