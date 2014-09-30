

#version 150 

in vec4 vertexPosition; 
in vec4 vertexTexCoord;

out vec2 tc;

void main() {  
  tc = vec2(vertexTexCoord.x, vertexTexCoord.y);
  gl_Position = vertexPosition;
}



