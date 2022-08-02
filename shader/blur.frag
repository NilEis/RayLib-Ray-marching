#version 400

uniform sampler2D texture0;
in vec4 fragColor;

in vec2 fragTexCoord;
uniform vec4 colDiffuse;

const highp float m[25] = float[25] (0.015026, 0.028569, 0.035391, 0.028569, 0.015026, 0.028569, 0.054318, 0.067288, 0.054318, 0.028569, 0.035391, 0.067288, 0.083355, 0.067288, 0.035391, 0.028569, 0.054318, 0.067288, 0.054318, 0.028569, 0.015026, 0.028569, 0.035391, 0.028569, 0.015026);
const int dim = 5;

void main() {
    vec4 texel = vec4(0.0);
    const int h = int(dim / 2);
    texel.a = 1.0;
    for(int y = -h; y <= h; y++) {
        for(int x = -h; x <= h; x++) {
            texel.rgb += texture(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y)).rgb * m[dim * (y + h) + (x + h)];
        }
    }
    gl_FragColor = texel;
}