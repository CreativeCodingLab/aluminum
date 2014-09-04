
#version 150

uniform sampler2D tex0;

in vec2 texCoord;
out vec4 outputFrag; 

void main(){

  vec4 outColor;
  outColor = vec4(texture(tex0, texCoord.st).xyz, 1.0);

  outputFrag = outColor;
}


