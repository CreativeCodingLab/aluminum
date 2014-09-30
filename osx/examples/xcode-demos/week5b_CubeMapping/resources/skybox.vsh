#version 150
uniform mat4 proj, view;

in vec4 vertexPosition;
out vec3 tc;

void main() {
  
  tc = vertexPosition.xyz;
  
  gl_Position = proj * view * vertexPosition;
  
}


