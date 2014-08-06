#version 100

varying highp vec4 f_position;
varying highp vec4 f_normal;
varying highp vec4 f_lightPosition;

void main(void) 
{
	const highp vec4 camera_pos = vec4(0.0,0.0,0.0,1.0);
    highp vec4 reflectedLight = normalize(reflect(f_position - f_lightPosition, f_normal));
    highp float color = dot(reflectedLight, normalize(camera_pos - f_position));
    gl_FragColor = vec4(color, color, color, 1.0);
}