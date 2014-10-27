#version 150

// define an input variable - this must match the name of an output variable in the vertex shader. In the vertex shader, it is linked to a specific vertex. Here in the fragment shader, it is interpolated across every pixel within the "primitive" as defined by out glDrawElements call on the CPU side. We are using GL_TRIANGLES, so the input variable will be a linear interpolation of three colors, that is, between each color assoicated with each vertex.
in vec4 color;


// define an output variable that, by default, is used to write RGBA information directly to the "frame buffer", i.e., the currently active window
out vec4 outputFrag; 

void main() {
  //set the color of the output pixel to be the value as interpolated across each of the colors associated with the triangle vertices.
  outputFrag = color;  
}
