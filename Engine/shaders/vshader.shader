#version 100

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTextureCoord;

uniform mat4 uWorldModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform vec3 uLightPosition;

varying vec2 vTextureCoord;
varying vec3 vPosition;
varying vec3 vNormal;
varying vec3 vLightPosition;

void main(void)
{
    vPosition = (uViewMatrix * uWorldModelMatrix * vec4(aPosition, 1.0)).xyz;
    vNormal = normalize(uViewMatrix * uWorldModelMatrix* vec4(aNormal, 0.0)).xyz;
	// Light position is already in world coordinates. So multiply by view matrix only
    vLightPosition = (uViewMatrix * vec4(uLightPosition, 1.0)).xyz; 
    vTextureCoord = aTextureCoord;

    gl_Position = uProjMatrix * vec4(vPosition, 1.0);
}