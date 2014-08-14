#version 100

varying highp vec3 vPosition;
varying highp vec3 vNormal;
varying highp vec3 vLightPosition;

uniform highp vec3 uAmbientLight;

void main(void) 
{
	const highp vec3 camera_pos = vec3(0.0, 0.0, 0.0);
	highp vec3 reflectedLight = vec3(0.0, 0.0, 0.0);
	highp vec3 lightDirection = vPosition - vLightPosition;
	// If light direction is oppsoite to normal calculate reflected light
	if (dot(lightDirection, vNormal) < 0.00001)
	{
		reflectedLight = normalize(reflect(vPosition - vLightPosition, vNormal));
	}
    highp float color = dot(reflectedLight, normalize(camera_pos - vPosition));
	if (color < 0.0) color = 0.0;
    gl_FragColor = vec4(color, color, color, 1.0) + vec4(uAmbientLight, 1.0);
}