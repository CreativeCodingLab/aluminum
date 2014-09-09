#version 150 
uniform mat4 proj, view, model;

uniform vec3 l1_position;
uniform vec3 l2_position;

in vec4 vertexPosition, vertexNormal; 
out vec3 N, L1, L2, V;


void main() {  
  
  //get the vertex position in CAMERA coordinates
  vec4 position = view * model * vertexPosition;
  
  
  vec3 P = position.xyz;
  
  //get the normalized vertex normal in CAMERA coordinates
  N = normalize(mat3(view) * mat3(model) * vertexNormal.xyz);
  
  //get the normalized vectors from each light position to the vertex positions
  L1 = normalize( (mat3(view) * mat3(model) * l1_position.xyz) - P);
  L2 = normalize( (mat3(view) * mat3(model) * l2_position.xyz) - P);
  
  //reverse direction of position vector to get view vector from vertex to camera
  V = normalize(-P);
  
  
  //of course, we always have to output our vertices in clip coords by multiplying through a projection matrix.
  gl_Position = proj * position; 
} 


