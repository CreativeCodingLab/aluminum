#version 150

out vec4 frag;
in float noiseVal;

void main(){
  vec3 color = vec3( 1.0 * ( 1.0 - (3.0 * noiseVal) ), 0.0, 0.0 );
  frag = vec4( color.rgb, 1.0 );
}
