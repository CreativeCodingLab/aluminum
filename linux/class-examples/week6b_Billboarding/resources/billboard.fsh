
#version 150

uniform sampler2D tex0;

in vec2 tc;
in vec3 particle_color;
out vec4 outputFrag;

void main(){
  
  float d = distance(tc, vec2(0.5,0.5));
  float p = 1.0 - (d * 2.0);
 
  outputFrag = vec4(mix(texture(tex0, tc).xyz, particle_color, 0.5), p);
}



  


