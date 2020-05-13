#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
uniform vec2 screenSize;

void main() {
    gl_Position = modelViewProjectionMatrix * position;
}