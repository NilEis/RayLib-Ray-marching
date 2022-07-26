#version 400
#define EPSILON 0.01
#define FARPLANE 150.0
#define MAX_MARCH_STEPS 250

#define SCENE_INVALID (-1)
#define SCENE_SPHERE 0
#define SCENE_PLANE 1

precision mediump float;

uniform float t;

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

struct Sphere {
    vec3 pos;
    float r;
    vec4 c;
};

struct Plane {
    vec3 normal;
    vec3 p;
    vec4 c;
};

const int spheres_size = 1;
Sphere spheres[spheres_size] = Sphere[spheres_size] (Sphere(vec3(0.0, 0.0, 5), 2.0, vec4(1.0, 0.0, 0.5, 1.0)));

const int planes_size = 1;
Plane planes[planes_size] = Plane[planes_size] (Plane(vec3(0.20, 1.0, 0.20), vec3(0.0, 0, 0.0), vec4(0.25, 0.25, 0.25, 1.0)));

vec3 sun = vec3(-1.5, 1.0, 0.0);
const vec4 ambient = vec4(0.25);

vec4 normal_to_rgb(vec3 v);
vec3 sceneSDF(vec3 p);
vec3 march(vec3 o, vec3 r);
vec3 estimateNormal(vec3 p);
float sdf_sphere(vec3 p, int i);
float sdf_plane(vec3 p, int i);
vec4 get_color(vec3 m, vec3 ray);

void main() {
    sun = vec3(sin(t) * -1.5, 1.0, cos(t));
    float rand = 0.0001;
    vec3 ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0, 2.0));
    vec3 m = march(vec3(0.0), ray);
    vec4 color = get_color(m, ray);
    /*ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0 + rand, (fragTexCoord.y * 2.0) - 1.0, 2.0));
    m = march(vec3(0.0), ray);
    color += get_color(m, ray);
    ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0 - rand, (fragTexCoord.y * 2.0) - 1.0, 2.0));
    m = march(vec3(0.0), ray);
    color += get_color(m, ray);
    ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0 + rand, 2.0));
    m = march(vec3(0.0), ray);
    color += get_color(m, ray);
    ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0 - rand, 2.0));
    m = march(vec3(0.0), ray);
    color += get_color(m, ray);*/
    gl_FragColor = color;
}

vec4 normal_to_rgb(vec3 v) {
    return vec4(normalize(v), 1.0);
}

vec3 sceneSDF(vec3 p) {
    float min = 999999999;
    float d = 0;
    int last_i = -1;
    int type = SCENE_INVALID;
    for(int i = 0; i < spheres_size; i++) {
        d = sdf_sphere(p, i);
        min = float(d < min) * d + float(min <= d) * min;
        last_i = int(d < min) * i + int(min <= d) * last_i;
        type = int(d <= EPSILON) * SCENE_SPHERE + int(EPSILON <= d) * type;
    }
    for(int i = 0; i < planes_size; i++) {
        d = sdf_plane(p, i);
        min = float(d < min) * d + float(min <= d) * min;
        last_i = int(d < EPSILON) * i + int(EPSILON <= d) * last_i;
        type = int(d < EPSILON) * SCENE_PLANE + int(EPSILON <= d) * type;
    }
    return vec3(min, last_i, type);
}

vec3 march(vec3 o, vec3 r) {
    float d = 1;
    for(int i = 0; i < MAX_MARCH_STEPS; i++) {
        vec3 sdf = sceneSDF(o + (d * r));
        float dist = sdf.x;
        if(dist < EPSILON) {
            return vec3(d, sdf.y, sdf.z);
        }
        d += dist;
        if(d >= FARPLANE) {
            return vec3(FARPLANE, -1, SCENE_INVALID);
        }
    }
    return vec3(FARPLANE, -1, SCENE_INVALID);
}

vec3 estimateNormal(vec3 p) {
    return normalize(vec3(sceneSDF(vec3(p.x + EPSILON, p.y, p.z)).x - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)).x, sceneSDF(vec3(p.x, p.y + EPSILON, p.z)).x - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)).x, sceneSDF(vec3(p.x, p.y, p.z + EPSILON)).x - sceneSDF(vec3(p.x, p.y, p.z - EPSILON)).x));
}

float sdf_sphere(vec3 p, int i) {
    return distance(spheres[i].pos, p) - spheres[i].r;
}

float sdf_plane(vec3 p, int i) {
    return dot(normalize(planes[i].normal), p - planes[i].p);
}

vec4 get_color(vec3 m, vec3 ray) {
    vec4 obj_color = vec4(0.9, 0, 1, 1);
    float s = (max(dot(estimateNormal(m.x * ray), sun), 0));
    if(march(m.x * ray, vec3(sun.x, sun.y, sun.z)).z == SCENE_INVALID) {
        s = 1.0;
    }
    switch(int(m.z)) {
        case SCENE_INVALID:
            obj_color = vec4(0.25);
            break;
        case SCENE_SPHERE:
            obj_color = spheres[int(m.y)].c;
            break;
        case SCENE_PLANE:
            obj_color = planes[int(m.y)].c;
            break;
    }
    return m.x < FARPLANE ? obj_color*s + ambient : vec4(0, 0, 0, 1.0);
}