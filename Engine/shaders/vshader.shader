#version 100

attribute vec3 position;
attribute vec3 normal;
attribute vec2 textureCoord;

uniform mat4 modelMatrix;
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec3 lightPosition;

varying vec2 f_textureCoord;
varying vec4 f_position;
varying vec4 f_normal;
varying vec4 f_lightPosition;

void main(void)
{
    gl_Position = modelMatrix * vec4(position, 1.0);
    gl_Position = worldMatrix * gl_Position;
    gl_Position = viewMatrix * gl_Position;
    gl_Position = projMatrix * gl_Position;
    f_position = viewMatrix * worldMatrix * modelMatrix * vec4(position, 1.0);
    f_normal = viewMatrix * worldMatrix * modelMatrix * vec4(normal, 0.0);
    f_normal = normalize(f_normal);
    f_lightPosition = viewMatrix * vec4(lightPosition, 1.0);
    f_textureCoord = textureCoord;
}