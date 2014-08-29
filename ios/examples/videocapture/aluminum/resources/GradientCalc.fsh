precision highp float;
#extension GL_OES_standard_derivatives : enable


uniform sampler2D tex0;
uniform vec2 CamDims;

//uniform sampler2D tex1;
varying vec2 ThetexCoord;


void main() {

    vec4 col;
    // Pixel step
    vec2 ps = 1.0/CamDims;
    vec2 texCoord;
    // Getting the 3 x 3 window grayvalues
    // I[x-1,y-1]
    texCoord.x  = ThetexCoord.x - ps.x;
    texCoord.y  = ThetexCoord.y - ps.y;
    col = texture2D(tex0, texCoord);
    float Xnm1Ynm1 = col.r*0.299 + col.g*0.587 + col.b*0.114;
    // I[x,y-1]
    texCoord.x  = ThetexCoord.x;
    texCoord.y  = ThetexCoord.y - ps.y;
    col = texture2D(tex0, texCoord);
    float XnYnm1 = col.r*0.299 + col.g*0.587 + col.b*0.114;
    // I[x+1,y-1]
    texCoord.x  = ThetexCoord.x + ps.x;
    texCoord.y  = ThetexCoord.y - ps.y;
    col = texture2D(tex0, texCoord);
    float Xnp1Ynm1 = col.r*0.299 + col.g*0.587 + col.b*0.114;
    
    // I[x-1,y]
    texCoord.x  = ThetexCoord.x - ps.x;
    texCoord.y  = ThetexCoord.y;
    col = texture2D(tex0, texCoord);
    float Xnm1Yn = col.r*0.299 + col.g*0.587 + col.b*0.114;
    // I[x,y-1]
    texCoord.x  = ThetexCoord.x;
    texCoord.y  = ThetexCoord.y;
    col = texture2D(tex0, texCoord);
    float XnYn = col.r*0.299 + col.g*0.587 + col.b*0.114;
    // I[x+1,y]
    texCoord.x  = ThetexCoord.x + ps.x;
    texCoord.y  = ThetexCoord.y;
    col = texture2D(tex0, texCoord);
    float Xnp1Yn = col.r*0.299 + col.g*0.587 + col.b*0.114;
    
    // I[x-1,y+1]
    texCoord.x  = ThetexCoord.x - ps.x;
    texCoord.y  = ThetexCoord.y + ps.y;
    col = texture2D(tex0, texCoord);
    float Xnm1Ynp1 = col.r*0.299 + col.g*0.587 + col.b*0.114;
    // I[x,y+1]
    texCoord.x  = ThetexCoord.x;
    texCoord.y  = ThetexCoord.y+ ps.y;
    col = texture2D(tex0, texCoord);
    float XnYnp1 = col.r*0.299 + col.g*0.587 + col.b*0.114;
    // I[x+1,y+1]
    texCoord.x  = ThetexCoord.x + ps.x;
    texCoord.y  = ThetexCoord.y+ ps.y;
    col = texture2D(tex0, texCoord);
    float Xnp1Ynp1 = col.r*0.299 + col.g*0.587 + col.b*0.114;
    
    
    // X gradient
    // according to:
    // http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html
    
    
    float GradX = (-3.0 * Xnm1Ynm1 + 0.0 * XnYnm1 + 3.0 * Xnp1Ynm1
                   -10.0* Xnm1Yn   + 0.0 * XnYn   +10.0 * Xnp1Yn
                   -3.0 * Xnm1Ynp1 + 0.0 * XnYnp1 + 3.0 * Xnp1Ynp1);
    
    float GradY = (-3.0 * Xnm1Ynm1 - 10.0 * XnYnm1 - 3.0 * Xnp1Ynm1
                   +0.0 * Xnm1Yn   +  0.0 * XnYn   + 0.0 * Xnp1Yn
                   +3.0 * Xnm1Ynp1 + 10.0 * XnYnp1 + 3.0 * Xnp1Ynp1);

    // limiting the value with non linear function:
    vec4 outcol;
    float pi = 3.14159265359;
    outcol.x = 0.5 + atan(GradX)/pi; // Between 0  and 1;
    outcol.y = 0.5 + atan(GradY)/pi; // Between 0  and 1;
    outcol.zw = vec2(0.0,1.0);
    
    gl_FragColor = outcol;

}
