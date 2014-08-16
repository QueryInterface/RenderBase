#version 100
precision highp float;

varying highp vec3 vPosition;
varying highp vec3 vNormal;
varying highp vec3 vLightPosition;

uniform highp vec3 uAmbientLight;

void main(void) 
{
    vec3 Normal         = normalize(vNormal);
    vec3 LightDirection = normalize(vLightPosition - vPosition);
    vec3 EyeDirection   = -normalize(vPosition);
    vec3 ReflectLight   = normalize(reflect(-LightDirection, Normal));

    vec4 IDiffuse       = 0.5 * vec4(1.0, 1.0, 1.0, 1.0) * max( dot(LightDirection, Normal), 0.0);
    IDiffuse            = clamp(IDiffuse, 0.0, 1.0);

    vec4 ISpecular      = 0.7 * vec4(1.0, 1.0, 1.0, 1.0) * pow( max( dot(ReflectLight, EyeDirection), 0.0), 10.0); //10.0 - material shininess power
    ISpecular           = clamp(ISpecular, 0.0, 1.0);

    gl_FragColor        = vec4(uAmbientLight, 0) + IDiffuse + ISpecular;
}