

#version 150 

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 lightPos;

in vec4 vertexPosition; 
in vec4 vertexNormal;

out vec3 normal;
out vec3 lightDir;


void main() {  
  vec4 position = view * model * vertexPosition;
  
  normal = vec3(view * model * vertexNormal);
  //lightDir = normalize(vec3(mv * vec4(lightPos.xyz, 1.0)) - position.xyz); 
  lightDir = normalize(vec3(view * vec4(lightPos.xyz, 1.0)) - position.xyz); 
  
  gl_Position = proj * position; 
} 



//in vec4 vertexPosition;
//in vec4 vertexNormal;

//out vec3 position;
//out vec3 normal;
//out vec3 lightDir;

//uniform vec3 lightPos;
//uniform mat4 mv;
//uniform mat4 proj;

//void main() {  
 
//  position = vec3(mv * vertexPosition);

//  normal = mv * vertexNormal;
 
//  lightDir = normalize(lightPos - position));  

//  gl_Position = proj * mv * vertexPosition; 
//} 

