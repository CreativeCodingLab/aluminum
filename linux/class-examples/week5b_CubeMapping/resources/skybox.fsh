#version 150

out vec4 frag;
in vec3 tc;

uniform samplerCube cube_texture;

void main(){
  frag = texture(cube_texture, tc);
}
