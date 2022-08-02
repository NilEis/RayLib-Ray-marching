#version 400

uniform sampler2D texture0;
in vec4 fragColor;

in vec2 fragTexCoord;
uniform vec4 colDiffuse;

const vec3 GAMMA = vec3(1.0 / 1.2);

vec4 gamma_correct(vec4 v);

void main() {
    vec4 texel = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    gl_FragColor = gamma_correct(texel);
}

vec4 gamma_correct(vec4 v) {
    v.rgb = pow(v.xyz, GAMMA);
    return v;
}