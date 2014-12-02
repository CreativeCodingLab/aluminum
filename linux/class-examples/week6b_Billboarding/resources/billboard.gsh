#version 150

uniform mat4 proj;

in vec4 vdata[];

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

out vec2 tc;
out vec3 particle_color;
void main (void)
{
  float particle_size = 1.0;
  particle_color = vdata[0].rgb;
  
  vec4 P = gl_in[0].gl_Position;
  
  // a: left-bottom
  vec2 va = P.xy + vec2(-0.5, -0.5) * particle_size;
  gl_Position = proj * vec4(va, P.zw);
  tc = vec2(0.0, 0.0);
  EmitVertex();
  
  // b: left-top
  vec2 vb = P.xy + vec2(-0.5, 0.5) * particle_size;
  gl_Position = proj * vec4(vb, P.zw);
  tc = vec2(0.0, 1.0);
  EmitVertex();
  
  // d: right-bottom
  vec2 vd = P.xy + vec2(0.5, -0.5) * particle_size;
  gl_Position = proj * vec4(vd, P.zw);
  tc = vec2(1.0, 0.0);
  EmitVertex();
  
  // c: right-top
  vec2 vc = P.xy + vec2(0.5, 0.5) * particle_size;
  gl_Position = proj * vec4(vc, P.zw);
  tc = vec2(1.0, 1.0);
  EmitVertex();
  
  EndPrimitive();
 }
