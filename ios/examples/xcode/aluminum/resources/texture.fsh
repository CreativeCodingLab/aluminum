
precision mediump float;

uniform sampler2D tex0;

varying vec2 texCoord;

void main() {

  vec4 outColor;

  //outColor = vec4(texCoord.st, 0.0, 1.0);
  outColor = vec4(texture2D(tex0, texCoord.st).xyz, 1.0);
    
    gl_FragColor = outColor;

    
}


