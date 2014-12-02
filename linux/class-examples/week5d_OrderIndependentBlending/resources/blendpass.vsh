

#version 330 core

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;


in vec4 vertexPosition;
out float z;

void main() {
  vec4 pos = proj * view * model * vertexPosition;
  z = pos.z;
  gl_Position = pos;
}

