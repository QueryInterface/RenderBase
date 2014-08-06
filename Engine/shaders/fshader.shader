#version 100

varying highp vec4 f_position;
varying highp vec4 f_normal;
varying highp vec4 f_lightPosition;

void main(void) 
{
    highp vec4 reflectedLight = reflect(f_position - f_lightPosition, f_normal);
    reflectedLight = normalize(reflectedLight);
    highp float color = dot(reflectedLight, normalize(vec4(0.0,0.0,0.0,1.0) - f_position));
    gl_FragColor = vec4(color, color, color, 1.0);
}