#version 330
in vec2 fragTexCoord;
out vec4 fragColor;
uniform sampler2D texture0;
uniform vec2 resolution;

void main(){
    vec2 texel = 1.0 / resolution;
    vec4 color = texture(texture0, fragTexCoord);
    color += texture(texture0, fragTexCoord + vec2(texel.x, 0.0));
    color += texture(texture0, fragTexCoord - vec2(texel.x, 0.0));
    color += texture(texture0, fragTexCoord + vec2(0.0, texel.y));
    color += texture(texture0, fragTexCoord - vec2(0.0, texel.y));
    fragColor = color / 5.0;
}