
#version 150

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;


in vec3 normal;
in vec3 lightDir;

out vec4 outputFrag; 

void main(){

  vec3 outColor;
  
  float diff = max(0.0, dot(normalize(normal), normalize(lightDir)));
  
  outColor = diff * diffuse;
  outColor += ambient;
  vec3 reflection = normalize(reflect(-normalize(lightDir),normalize(normal)));
  float spec = max(0.0, dot(normalize(normal), reflection)); 
  if(diff != 0.0) { 
    float fSpec = pow(spec, 64.0); 
    outColor.rgb += (vec3(fSpec, fSpec, fSpec) * specular.rgb); 
  } 

  //outColor = (vec3(normal.rgb) + 1.0 ) / 2.0;
  //outColor = (vec3(lightDir.rrr) + 1.0 ) / 2.0;
  //outColor = (vec3(reflection.rrr) + 1.0 ) / 2.0;

  outputFrag = vec4(outColor,1.0); 

}
