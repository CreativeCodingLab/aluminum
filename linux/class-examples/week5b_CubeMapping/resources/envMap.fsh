#version 150

in vec3 V;
in vec3 N;
uniform samplerCube cube_texture;
uniform mat4 view; // view matrix
uniform vec4 baseColor;
out vec4 frag;

void main () {
  /* reflect ray around normal from eye to surface */
  vec3 R = reflect (normalize(V), normalize(N));
 
  // convert from eye to world space
  R = vec3 ( inverse(view) * vec4(R, 0.0));

  frag = clamp(texture (cube_texture, R) + baseColor, 0.0, 1.0);
}