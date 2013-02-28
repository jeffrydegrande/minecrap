#version 330

layout(location = 0)in vec4 position;
layout(location = 1)in vec4 normal;
layout(location = 2)in vec4 color;

smooth out vec4 theColor;

uniform mat4 perspectiveMatrix;
uniform mat4 cameraMatrix;

void main() {
    gl_Position = perspectiveMatrix * cameraMatrix * position;
    theColor = color;
}
