#version 400

uniform sampler2D texture0;
in vec4 fragColor;

in vec2 fragTexCoord;
uniform vec4 colDiffuse;

void main() {
    vec4 texel = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    gl_FragColor = texel;
}