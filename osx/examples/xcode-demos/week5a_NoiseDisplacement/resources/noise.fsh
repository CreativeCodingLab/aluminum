#version 150

out vec4 frag;
in float noiseVal;
in float noiseVal2;

void main(){
  vec3 color = vec3( 1.0 * ( 1.0 - (3.0 * noiseVal) ), 0.0, 0.0 );
  vec3 color2 = vec3( 0.0, 1.0 * ( 1.0 - (3.0 * noiseVal2) ), 0.0 );
  
  frag = vec4( color.rr, color2.g, 1.0 );
  //frag = vec4(1.0,0.0,0.0,1.0);
}
