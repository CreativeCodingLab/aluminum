#version 150 
uniform mat4 proj, view, model;

uniform vec4 l1_position;
uniform vec4 l2_position;
uniform float t1;
uniform float t2;

in vec4 vertexPosition, vertexNormal; 
out vec3 N, L1, L2, V;


void main() {
  
  //displace the vertex along the normal based on the t values we pass in and a trigonometric function
  float displacement = (sin(vertexPosition.y * t1/1.0) + cos(vertexPosition.x * t2/2.0)) * 0.2;
  vec4 position = view * model * (vertexPosition + (vertexNormal * displacement));
 
  //just use xyz to calculate vectors between vertex, light, and camera
  vec3 P = position.xyz;
  
  //get the normalized vertex normal in CAMERA coordinates
  N = normalize(mat3(view) * mat3(model) * vertexNormal.xyz);
  
  //the lights positions are defined in WORLD coordinates, we want to put them in CAMERA coordinates too
  vec4 L1_cam = view * l1_position;
  vec4 L2_cam = view * l2_position;
  
  //get the normalized vectors from each light position to the vertex positions
  L1 = vec3(normalize(L1_cam - position).xyz);
  L2 = vec3(normalize(L2_cam - position).xyz);
  
  
  //reverse direction of position vector to get view vector from vertex to camera
  V = normalize(-P);
  
  
  //of course, we always have to output our vertices in clip coords by multiplying through a projection matrix.
  gl_Position = proj * position; 
} 


