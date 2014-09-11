
#version 150

uniform sampler2D tex0;

in vec2 texCoord;
out vec4 outputFrag; 

void main(){

  vec4 pixel = texture(tex0, texCoord);
  outputFrag = pixel;
}


