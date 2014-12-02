
#version 330 core

uniform vec4 inColor;

layout(location = 0) out vec4 c0;
layout(location = 1) out vec4 c1;

uniform float divVal;
uniform float powVal;
uniform bool zDepthOn;

uniform float zval;
in float z;

void main(){

  vec4 color = inColor;
  
  // Insert your favorite weighting function here. The color-based factor
  // avoids color pollution from the edges of wispy clouds. The z-based
  // factor gives precedence to nearer surfaces.

  float useDivVal = divVal;
  float usePowVal = powVal;
  
  float usez = zval;
  if (zDepthOn == true) {
    usez = z;
  }
  
  float minOf = min(1.0, max(max(color.r, color.g), color.b) * color.a);
  float denominator = (1e-5 + pow(abs(usez) / useDivVal, usePowVal));
  float weight = max(minOf, color.a) * clamp(0.03 / denominator, 1e-2, 3e3);
  
  // Blend Func: GL_ONE, GL_ONE
  // Switch to premultiplied alpha and weight
  c0 = vec4(color.rgb * color.a, color.a) * weight;
  
  // Blend Func: GL_ZERO, GL_ONE_MINUS_SRC_ALPHA
  c1 = vec4(color.a);
  
 }


