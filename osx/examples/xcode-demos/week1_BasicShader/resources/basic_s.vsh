#version 150 

//uniform variables receive data from the CPU, they must be explicity linked to this shader program using the glUniform commands when it is bound via the glUseProgram command. The uniforms are in scope for as long as the shader program is bound.
uniform mat4 mv; //a matrix that defines how points in object space are transformed to camera space (or eye space)
uniform mat4 proj; //a matrix that defines how a points are projected from 3D to 2D

//attribute variables are associated with specifc vertices as defined by the currently bound vertex array object, which contains data defined in a vertex buffer object and information about how to read through that data.
in vec4 vertexPosition; 
in vec4 vertexColor; 

//an output variable that gets passed to the fragment shader
out vec4 color;

void main() {  
  color = vertexColor; //simply "pass-through" our input color to the fragment shader
  
  //the last line of the vertex shader needs to define a special position variable that outputs the input vertexPosition in "normalized deviced coordinates" that range between -1 and 1 in the x and y directions.
  
  //here, reading from right to left, we:
  // 1. multiply the vertex position by our "camera" matrix to position the vertex in terms of camera. That is, we perfrom a change of basis transformation where the camera becomes the origin of our space;
  // 2. multiple the vertex, now in its new "camera coordinates" or "eye space", by the our projection matrix to "flatten" it into 2D and "squish" it into coordinates that range between -1 and +1.
  gl_Position = proj * mv * vertexPosition;
  
  
} 

