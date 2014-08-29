precision highp float;
#extension GL_OES_standard_derivatives : enable


uniform sampler2D tex0;


varying vec2 ThetexCoord;


void main() {

    vec4 col;

    vec2 texCoord;
    texCoord.x  = ThetexCoord.x;
    texCoord.y  = ThetexCoord.y;
    col = texture2D(tex0, texCoord);
    float XnYn = col.r*0.299 + col.g*0.587 + col.b*0.114;
    float Alpha = 0.3;
    float Imin = 0.0;
    float Imax = 1.0;
    float OutValue = Imin + (Imax - Imin)*pow(XnYn,Alpha);
    gl_FragColor = vec4(OutValue,OutValue,OutValue,1.0);

}
