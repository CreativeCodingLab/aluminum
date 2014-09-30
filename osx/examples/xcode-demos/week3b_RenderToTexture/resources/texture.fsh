
#version 150

uniform sampler2D tex0;
smooth in vec2 texCoord;
out vec4 outputFrag; 

void main(){

  vec3 outColor = vec3(texture(tex0, texCoord.st).xyz);
  outputFrag = vec4(outColor, 1.0);

}
