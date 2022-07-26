#version 400
#define EPSILON 0.0001
#define FARPLANE 15.0
#define MAX_MARCH_STEPS 10

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

struct Sphere {
    vec3 pos;
    float r;
    vec4 c;
};

const int spheres_size = 1;
Sphere spheres[spheres_size] = Sphere[spheres_size] (Sphere(vec3(0.0, 0.0, 5), 2.0, vec4(1.0, 0.0, 0.5, 1.0)));

const vec3 sun = vec3(-1.5, 1.0, 0.0);
const vec4 ambient = vec4(0.25);

vec4 normal_to_rgb(vec3 v);
vec2 sceneSDF(vec3 p);
vec2 march(vec3 o, vec3 r);
vec3 estimateNormal(vec3 p);

void main() {
    float rand = 0.001;
    vec3 ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0, 2.0));
    vec2 m = march(vec3(0.0), ray);
    vec4 color = m.x < FARPLANE ? (spheres[int(m.y)].c) * (max(dot(estimateNormal(vec3(0.0) + m.x * ray), sun), 0)) + ambient : vec4(0, 0, 0, 1.0);
    ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0 + rand, (fragTexCoord.y * 2.0) - 1.0, 2.0));
    m = march(vec3(0.0), ray);
    color += m.x < FARPLANE ? (spheres[int(m.y)].c) * (max(dot(estimateNormal(vec3(0.0) + m.x * ray), sun), 0)) + ambient : vec4(0, 0, 0, 1.0);
    ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0 - rand, (fragTexCoord.y * 2.0) - 1.0, 2.0));
    m = march(vec3(0.0), ray);
    color += m.x < FARPLANE ? (spheres[int(m.y)].c) * (max(dot(estimateNormal(vec3(0.0) + m.x * ray), sun), 0)) + ambient : vec4(0, 0, 0, 1.0);
    ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0 + rand, 2.0));
    m = march(vec3(0.0), ray);
    color += m.x < FARPLANE ? (spheres[int(m.y)].c) * (max(dot(estimateNormal(vec3(0.0) + m.x * ray), sun), 0)) + ambient : vec4(0, 0, 0, 1.0);
    ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0 - rand, 2.0));
    m = march(vec3(0.0), ray);
    color += m.x < FARPLANE ? (spheres[int(m.y)].c) * (max(dot(estimateNormal(vec3(0.0) + m.x * ray), sun), 0)) + ambient : vec4(0, 0, 0, 1.0);
    gl_FragColor = color * 0.2;
}

vec4 normal_to_rgb(vec3 v) {
    return vec4(normalize(v), 1.0);
}

vec2 sceneSDF(vec3 p) {
    float min = 999999999;
    float d = 0;
    int last_i = -1;
    for(int i = 0; i < spheres_size; i++) {
        d = distance(spheres[i].pos, p) - spheres[i].r;
        min = float(d < min) * d + float(min <= d) * min;
        last_i = int(d < min) * i + int(min <= d) * last_i;
    }
    return vec2(min, last_i);
}

vec2 march(vec3 o, vec3 r) {
    float d = 0;
    for(int i = 0; i < MAX_MARCH_STEPS; i++) {
        vec2 sdf = sceneSDF(o + (d * r));
        float dist = sdf.x;
        if(dist < EPSILON) {
            return vec2(d, sdf.y);
        }
        d += dist;
        if(d >= FARPLANE) {
            return vec2(FARPLANE, -1);
        }
    }
    return vec2(FARPLANE, -1);
}

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(sceneSDF(vec3(p.x + EPSILON, p.y, p.z)).x - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)).x, sceneSDF(vec3(p.x, p.y + EPSILON, p.z)).x - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)).x, sceneSDF(vec3(p.x, p.y, p.z + EPSILON)).x - sceneSDF(vec3(p.x, p.y, p.z - EPSILON)).x));
}