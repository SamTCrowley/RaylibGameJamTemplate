#version 100
precision mediump float;

varying vec2 fragTexCoord;
uniform sampler2D texture0;
uniform vec2 resolution;

void main(){
    vec2 texel = 1.0 / resolution;
    vec4 color = texture2D(texture0, fragTexCoord);
    color += texture2D(texture0, fragTexCoord + vec2(texel.x, 0.0));
    color += texture2D(texture0, fragTexCoord - vec2(texel.x, 0.0));
    color += texture2D(texture0, fragTexCoord + vec2(0.0, texel.y));
    color += texture2D(texture0, fragTexCoord - vec2(0.0, texel.y));
    gl_FragColor = color / 5.0;
}