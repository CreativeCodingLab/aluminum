uniform mat4 proj;
uniform mat4 mv;
uniform vec2 MouseCords;


attribute vec4 vertexPosition;
attribute vec3 vertexTexCoord;
varying vec2 ThetexCoord;

//attribute vec4 vertexColor;





void main() {
    ThetexCoord = vertexTexCoord.xy;
    gl_Position = proj * mv * vertexPosition;
} 

