precision mediump float;
//varying lowp vec4 color;
varying lowp vec2 texCoord;
uniform sampler2D tex0;

//vec3 TextuValues[7*7];

float lowPasFilter(vec2 CentralCords, int sizeX, int sizeY,float stepX, float stepY){
    
    float theValue =0.0;
    
    for (int x =0; x< sizeX; x++) {
        for (int y = 0; y < sizeY; y++) {
            vec2 currentcords = texCoord.st + vec2((float(x)-float(sizeX)/2.0)*stepX,(float(y)-float(sizeY)/2.0)*stepY);
            
            vec4 colorval = texture2D(tex0, currentcords);
         //   float Grayval = dot(colorval.rgb,vec3(0.299,0.587,0.114));//colorval.r*0.299 + colorval.g*0.587 + colorval.b*0.114;
            float Grayval = colorval.r;
            theValue = Grayval;
        }
    }
    
    return theValue;//*(1./(float(sizeX)*float(sizeY)));
}


void main() {
    
    // fetching the testure values:
    
//    for (int x =0; x< 7; x++) {
//        for (int y = 0; y < 7; y++) {
//           // vec2 currentcords = texCoord.st + vec2((float(x)-float(25.)/2.0)*(1./640.),(float(y)-float(25.)/2.0)*(1./480.));
//            TextuValues[x*7+y] = vec3(1.0,1.0,1.0);//(texture2D(tex0, texCoord.st)).xyz;
//        }
//   }
    
    
    float Grayval = lowPasFilter(texCoord, 13, 13,1./640.0, 1./480.0);
    gl_FragColor = vec4(Grayval,Grayval,Grayval,1.0);
}
