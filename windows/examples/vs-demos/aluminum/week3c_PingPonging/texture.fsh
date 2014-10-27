
#version 150

uniform sampler2D tex0;
//uniform float bloom;

in vec2 texCoord;
out vec4 outputFrag; 

void main(){

  vec4 outColor = vec4(1.0,0.0,0.0,1.0);
  vec4 pixel = texture(tex0, texCoord);
    outputFrag = pixel; //vec4(1.0,0.0,0.0,1.0);
}


