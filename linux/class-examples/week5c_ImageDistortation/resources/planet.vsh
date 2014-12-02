#version 150

in vec4 vertexPosition;

uniform vec4 rot;

out vec2 pos;

void main() {
  pos.x = dot(vertexPosition.xy, rot.xy);
	pos.y = dot(vertexPosition.xy, rot.zw);
  gl_Position = vertexPosition;
 }


