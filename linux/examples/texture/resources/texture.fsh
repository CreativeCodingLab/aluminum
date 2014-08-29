
#version 150

uniform sampler2D tex0;
uniform float bloom;

in vec2 texCoord;
out vec4 outputFrag; 

void main(){

  vec4 outColor;

  // //debugs...
  //outColor = vec4(texCoord.st, 0.0, 1.0);
  //outColor = vec4(texture(tex0, texCoord.st).xyz, 1.0);
  // //end debugs

  vec4 sum = vec4(0);
  int j,i;

  for(i= -5 ;i < 5; i++) {
    for (j = -5; j < 5; j++) {
      sum += texture(tex0, texCoord.st + vec2(j, i)*0.0015) * 0.20;           
    }
  }

  outColor = (sum*sum* bloom) + texture(tex0, texCoord);

  outputFrag = outColor;
}
