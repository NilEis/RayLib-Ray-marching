#version 400

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

vec4 normal_to_rgb(vec3 v);
bool sphere_ray_intersect(vec3 origin, vec3 ray);

vec4 sphere = vec4(0.0, 0.0, 5.0, 2.0);

void main() {
    vec3 ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0, 1.0));
    vec4 color = vec4(1.0);
    if(sphere_ray_intersect(vec3(0.0), ray)) {
        color = vec4(0, 0, 0, 0.25);
    }
    gl_FragColor = color;
}

vec4 normal_to_rgb(vec3 v) {
    return vec4(normalize(v), 1.0);
}

bool sphere_ray_intersect(vec3 origin, vec3 ray) {
    float delta;
    vec3 d_sphere_origin = origin - sphere.xyz;
    delta = pow(dot(ray, d_sphere_origin), 2.0) - (pow(length(d_sphere_origin), 2.0) - pow(sphere.w, 2.0));
    return !(delta < 0);
}