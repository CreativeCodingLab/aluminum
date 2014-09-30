#version 150

in vec2 pos;

uniform samplerCube cube_texture;
uniform float zoom;
uniform float power;

out vec4 frag;


void main(){
  
  vec3 texCoord = vec3(0.0,0.0,0.0);
  
	texCoord.xz = pos;
	texCoord.y = pow(dot(pos, pos), power) * zoom - 1.0;
  frag = texture(cube_texture, texCoord);
}
