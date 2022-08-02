#version 400

uniform sampler2D texture0;
in vec4 fragColor;

in vec2 fragTexCoord;
uniform vec4 colDiffuse;

vec4 aces_approx(vec4 v);

void main() {
    vec4 texel = aces_approx(texture(texture0, fragTexCoord) * colDiffuse * fragColor);
    gl_FragColor = texel;
}

vec4 aces_approx(vec4 v) {
    v.rgb *= 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    v.rgb = clamp((v.rgb * (a * v.rgb + b)) / (v.rgb * (c * v.rgb + d) + e), 0.0f, 1.0f);
    return v;
}