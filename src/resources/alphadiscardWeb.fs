#version 100
precision mediump float;


varying vec2 fragTexCoord;

// Input uniform values
uniform sampler2D texture0;


void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    if (texelColor.a == 0.0) discard;
    gl_FragColor = texelColor;
}