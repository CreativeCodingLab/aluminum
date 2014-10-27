

#version 150 

uniform mat4 view;
uniform mat4 model;

in vec4 vertexPosition; 
in vec4 vertexTexCoord;

out vec4 vdata;

void main() {
  gl_Position = view * model * vertexPosition; //point in eye space
   vdata = vertexTexCoord;

}

