precision highp float;
#extension GL_OES_standard_derivatives : enable


uniform sampler2D tex0;
//uniform sampler2D tex1;
varying vec2 texCoord;
varying lowp vec4 color;

void main() {
    
    //Javier original code
   /*
    vec4 outColor; // = color;
    vec4 GridOut = texture2D(tex1, texCoord.st);
     // outColor = texture2D(tex0, texCoord.st);
//    if (outColor.a ==0.0){
//        discard;
//    }
//    else{
    GridOut = vec4(vec3(GridOut.xyz), 0.4*GridOut.w);
//    }
    //gl_FragColor = GridOut;
    
    */
    
    
    
    
    int frequency = 120;
    //vec4 c0 = vec4(1.0,0.0,0.0,1.0);
    //vec4 c1 = vec4(0.0,0.0,0.0,1.0);
    
    vec4 col = texture2D(tex0, texCoord);
    
    float tcx = mod(floor(texCoord.x * float(frequency * 2)), 10.0);
    float tcy = mod(floor(texCoord.y * float(frequency * 2)), 10.0);
    
    
    if (tcx == 0.0 || tcy == 0.0) {
        gl_FragColor = col; //vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        gl_FragColor = vec4(1.0,1.0,1.0,1.0);
    }
  
    
    //float delta = abs(tc.x - tc.y) ;
    //float delta = abs(tc.x) ;
    
    //gl_FragColor = vec4(tc.x, tc.y, 0.0, 1.0);
    //gl_FragColor = mix(c0, c1, delta);
    

    
    
    
    
}
