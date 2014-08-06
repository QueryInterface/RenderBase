#version 100

attribute vec3 position;
attribute vec3 normal;
attribute vec2 textureCoord;

uniform mat4 model_worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform vec3 lightPosition;

varying vec2 f_textureCoord;
varying vec4 f_position;
varying vec4 f_normal;
varying vec4 f_lightPosition;

void main(void)
{
    f_position = viewMatrix * model_worldMatrix * vec4(position, 1.0);
    f_normal = viewMatrix * model_worldMatrix* vec4(normal, 0.0);
	// Light position is already in world coordinates. So multiply by view matrix only
    f_lightPosition = viewMatrix * vec4(lightPosition, 1.0); 
    f_textureCoord = textureCoord;

    gl_Position = projMatrix * f_position;
}