// version 150 

// uniform mat4 proj;
// uniform mat4 view;
// uniform mat4 model;

// in vec4 vertexPosition; 
// in vec4 vertexTexCoord;

// out vec2 texCoord;

// void main() {  
//   texCoord = vertexTexCoord.xy;
//   gl_Position = proj * view * model * vertexPosition; 
// } 

#version 150

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

in vec4 vertexPosition; 
in vec4 vertexTexCoord;

out vec2 texCoord;

void main() {  
//  texCoord = vec2(vertexTexCoord.x, 1.0 - vertexTexCoord.y); //flips upside down webcam
    texCoord = vec2(vertexTexCoord.x, vertexTexCoord.y); //flips upside down webcam
  gl_Position = proj * view * model * vertexPosition;
} 