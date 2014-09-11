#version 150 
uniform mat4 proj, view, model;

vec3 l1_position = vec3(10.0, 10.0, 0.0);

in vec4 vertexPosition, vertexNormal, vertexTexCoord;

out vec3 N, L1, V;
out vec2 texCoord;


void main() {  
  
  
  //get the vertex position in CAMERA coordinates
  vec4 position = view * model * vertexPosition;

  //just use xyz to calculate vectors between vertex, light, and camera
  vec3 P = position.xyz;
  
  //get the normalized vertex normal in CAMERA coordinates
  N = normalize(mat3(view) * mat3(model) * vertexNormal.xyz);
  
  //the lights positions are defined in WORLD coordinates, we want to put them in CAMERA coordinates too
  L1 = mat3(view) * l1_position.xyz;
  
  //get the normalized vectors from each light position to the vertex positions
  L1 = normalize(L1 - P);
  
  //reverse direction of position vector to get view vector from vertex to camera
  V = normalize(-P);
  
  //pass texture coordinates to fragment shader
  texCoord = vertexTexCoord.xy;

  //of course, we always have to output our vertices in clip coords by multiplying through a projection matrix.
  gl_Position = proj * position; 
} 


