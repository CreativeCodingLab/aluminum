#version 330 core

layout(location = 0) in vec4 vertexPosition;  //object space vertex position

uniform mat4 MVP;  //combined modelview projection matrix

void main()
{
    //get clipspace position
    gl_Position = MVP*vertexPosition;
}