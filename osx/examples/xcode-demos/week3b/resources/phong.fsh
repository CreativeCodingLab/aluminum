
#version 150

uniform sampler2D tex0;

in vec2 texCoord;
in vec3 V, N, L1;



float spec_intensity = 32.0;
vec3 l1_diffuse = vec3(1.0,0.0,0.0);
vec3 l1_specular = vec3(1.0,1.0,1.0);

out vec4 frag;

void main(){
  
  vec4 outColor1 = vec4(0.0);
  
  //diffuse light depends on the angle between the light and the vertex normal
  float diff1 = max(0.0, dot(N, L1)); //just to make sure not negative
  vec3 color1 = diff1 * l1_diffuse;
  
  //specular highlights depend upon the position/orientation of the camera and the direction of the light reflecting off of this geometry
  vec3 R1 = normalize(-reflect(L1,N)); //get light vector reflected across the plane defined by the normal of this geometry
  float spec1 = pow( max(dot(R1, V), 0.0), spec_intensity); //raising the value to a particular intensity value shrinks the size of the specular highlight so that only a reflection vector (R1) that is very close to the view vector (V) will be applied to this fragment.
  
  color1 += spec1 * l1_specular;
  if (spec1 > 1.0) {
    outColor1 = vec4(l1_specular,1.0);
  } else {
    outColor1 = clamp(vec4(color1,1.0), 0.0,1.0);
  }
  
  //get pixel from texture at this texCoord
  vec4 outColor2 = vec4(vec3(texture(tex0, texCoord.st).xyz), 1.0);

  
  vec3 ambient = vec3(0.1,0.1,0.1); //just a default ambient color
  frag = clamp(vec4(ambient, 1.0) + outColor1 + outColor2, 0.0, 1.0); //add the two lights together, make sure final value is between 0.0 and 1.0
  
}
