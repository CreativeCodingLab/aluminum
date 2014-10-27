
#version 330 core

uniform sampler2D tex0;
uniform sampler2D tex1;

in vec2 texCoord;
out vec4 outputFrag; 


void main(){

  //vec4 accum = texelFetch(tex0, ivec2(gl_FragCoord.xy), 0);
  //float reveal = texelFetch(tex1, ivec2(gl_FragCoord.xy), 0).r;
  vec4 accum = texture(tex0, texCoord);
  float reveal = texture(tex1, texCoord).r;
  
  // Blend Func: GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA
  outputFrag = vec4(accum.rgb / max(accum.a, 1e-5), reveal); //blog version
  //outputFrag = vec4(accum.rgb / clamp(accum.a, 1e-4, 5e4), reveal); //paper version
}


