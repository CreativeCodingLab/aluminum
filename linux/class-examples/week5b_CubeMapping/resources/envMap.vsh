#version 150
uniform mat4 proj, view, model;

in vec3 vertexPosition;
in vec3 vertexNormal;
out vec3 tc;

out vec3 V;
out vec3 N;

void main () {
  V = (vec3 (view * model * vec4(vertexPosition.xyz,1.0)));
  N = (vec3 (view * model * vec4(vertexNormal.xyz,0.0)));
  gl_Position = proj * view * model * vec4(vertexPosition,1.0);
}

