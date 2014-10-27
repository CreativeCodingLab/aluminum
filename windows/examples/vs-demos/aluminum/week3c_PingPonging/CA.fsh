
#version 150

uniform sampler2D tex;
uniform float du;
uniform float dv;

in vec2 tc;
out vec4 outputFrag; 

void main() {
    
    vec4 C = texture( tex, tc );
    vec4 E = texture( tex, vec2(tc.x + du, tc.y) );
    vec4 N = texture( tex, vec2(tc.x, tc.y + dv) );
    vec4 W = texture( tex, vec2(tc.x - du, tc.y) );
    vec4 S = texture( tex, vec2(tc.x, tc.y - dv) );
    vec4 NE = texture( tex, vec2(tc.x + du, tc.y + dv) );
    vec4 NW = texture( tex, vec2(tc.x - du, tc.y + dv) );
    vec4 SE = texture( tex, vec2(tc.x + du, tc.y - dv) );
    vec4 SW = texture( tex, vec2(tc.x - du, tc.y - dv) );
   
    int count = 0;
    
    if (E.r == 1.0) { count++; }
    if (N.r == 1.0) { count++; }
    if (W.r == 1.0) { count++; }
    if (S.r == 1.0) { count++; }
    if (NE.r == 1.0) { count++; }
    if (NW.r == 1.0) { count++; }
    if (SE.r == 1.0) { count++; }
    if (SW.r == 1.0) { count++; }
    
    
    if ( (C.r == 0.0 && count == 3) ||
        (C.r == 1.0 && (count == 2 || count == 3))) {
        outputFrag = vec4(1.0, 1.0, 1.0, 1.0); //cell lives...
    } else {
        outputFrag = vec4(0.0, 0.0, 0.0, 1.0); //cell dies...
    }
    
}


