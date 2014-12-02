
#version 330 core

uniform sampler2D tex0;

in vec2 texCoord;
out vec4 outputFrag; 

void main() {
  outputFrag = texture(tex0, texCoord);
 // outputFrag = vec4(1.0, vec2(texture(tex0, texCoord).rg), 1.0);

}


