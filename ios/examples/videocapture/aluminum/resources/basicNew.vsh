uniform mat4 proj;
uniform mat4 mv;
uniform vec2 MouseCords;

attribute vec4 vertexPosition;
attribute vec3 vertexTexCoord;
varying vec2 texCoord;
uniform sampler2D tex0;
//attribute vec4 vertexColor;
varying vec4 color;

// TO DO
// Low pass FIlter gray input



void main() {
    
    //Javier's original code
    /*
    float sigma = 0.1;
    texCoord = vertexTexCoord.xy;
    vec4 newPos = vertexPosition;
    float gauss1 = exp(-0.5*(((newPos.x-MouseCords.x)/sigma)*((newPos.x-MouseCords.x)/sigma) +
                             ((newPos.y+MouseCords.y)/sigma)*((newPos.y+MouseCords.y)/sigma)));
    
    
    
    vec4 outColor; // = color;
    outColor = texture2D(tex0, texCoord.st);
    float Grayval = outColor.r*0.299 + outColor.g*0.587 + outColor.b*0.114;
    newPos.z = max((1.0 -Grayval),gauss1);
   
    color = vec4(Grayval,Grayval,Grayval,1.0);
    
  gl_Position = proj * mv * newPos;
    */
    
    
    
    //angus futzing around...
    
    float sigma = 0.1;
    texCoord = vertexTexCoord.xy;
    vec4 newPos = vertexPosition;
    float gauss1 = exp(-0.5*(((newPos.x-MouseCords.x)/sigma)*((newPos.x-MouseCords.x)/sigma) +
                             ((newPos.y+MouseCords.y)/sigma)*((newPos.y+MouseCords.y)/sigma)));
    
    
    
    vec4 outColor; // = color;
    outColor = texture2D(tex0, texCoord.st);
    float Grayval = outColor.r*0.299 + outColor.g*0.587 + outColor.b*0.114;
   // newPos.x += max((1.0 -Grayval),gauss1);
    
    color = vec4(Grayval,Grayval,Grayval,1.0);
    
   // newPos.y += gauss1;
    texCoord.x += outColor.x * 0.1;
    texCoord.y += outColor.y * 0.1;
    
    gl_Position = proj * mv * newPos;
    
    
} 

