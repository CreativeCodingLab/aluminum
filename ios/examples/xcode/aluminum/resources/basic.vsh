uniform mat4 proj;
uniform mat4 mv;

attribute vec4 vertexPosition; 
attribute vec4 vertexColor; 
varying vec4 color; 

void main() {  
  color = vertexColor; 
  gl_Position = proj * mv * vertexPosition; 
} 

