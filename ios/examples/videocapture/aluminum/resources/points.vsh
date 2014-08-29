uniform mat4 proj;
uniform mat4 mv;

attribute vec4 vertexPosition;

void main() {  

    gl_PointSize = 5.0;
  gl_Position = proj * mv * vertexPosition; 
} 

