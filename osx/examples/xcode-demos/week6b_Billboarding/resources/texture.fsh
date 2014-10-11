
#version 150

uniform sampler2D tex0;
uniform float du;
uniform float dv;
uniform float gamma;
uniform bool invert;
uniform bool off;
uniform bool emboss;


in vec2 texCoord;
out vec4 outputFrag;


void main(){
  
  if (!off) {
    
    vec4 E = texture( tex0, vec2(texCoord.x + du, texCoord.y) );
    vec4 N = texture( tex0, vec2(texCoord.x, texCoord.y + dv) );
    vec4 W = texture( tex0, vec2(texCoord.x - du, texCoord.y) );
    vec4 S = texture( tex0, vec2(texCoord.x, texCoord.y - dv) );
    vec4 NE = texture( tex0, vec2(texCoord.x + du, texCoord.y + dv) );
    vec4 NW = texture( tex0, vec2(texCoord.x - du, texCoord.y + dv) );
    vec4 SE = texture( tex0, vec2(texCoord.x + du, texCoord.y - dv) );
    vec4 SW = texture( tex0, vec2(texCoord.x - du, texCoord.y - dv) );
    
    vec3 v;
    
    
    if (emboss) {
      
      //emboss
      // -1, -1,  0,
      // -1,  0,  1,
      // 0,  1,  1
      
      vec3 edges = vec3(0.5,0.5,0.5) + (NW*-1.0 + N*-1.0 + W*-1.0 + E + S + SE).rgb;
      v = edges;
      
    } else {
      
      
      //edge detection
      
      //    -1 -2 -1       1 0 -1
      // H = 0  0  0   V = 2 0 -2
      //     1  2  1       1 0 -1
      //
      // result = sqrt(H^2 + V^2)
      
      
      
      
      
      vec4 horizEdge = SW + S*2.0 + SE - NW - N*2.0 - NE;
      vec4 vertEdge  = SW + W*2.0 + NW - SE - E*2.0 - NW;
      
      vec3 edges = vec3(sqrt((horizEdge.rgb * horizEdge.rgb) + (vertEdge.rgb * vertEdge.rgb)));
      
      //v = edges;
      
      vec3 gammaV = vec3(gamma, gamma, gamma); // 0.0 - 20.0, default 1.0
      
      v = pow( edges, gammaV );
      
      if (invert) {
        v = mix( v, 1.0 - v, 1.0 );
      }
      
      
    }
    outputFrag = vec4(v.rgb, 1.0);
    
  } else {
    
    outputFrag = texture( tex0, vec2(texCoord.x, texCoord.y) );
  }
  
}


