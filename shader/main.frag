#version 400

#define SCENE_INVALID (-1)
#define SCENE_SPHERE 0
#define SCENE_PLANE 1
#define SCENE_MANDEL 2

precision lowp float;

uniform float t;
uniform float EPSILON = 0.0009;
uniform float FARPLANE = 15.0;
uniform int MAX_MARCH_STEPS = 250;

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

struct Hit {
    float d;
    int iterations;
    int index;
    int type;
    vec3 point;
    vec3 normal;
} m_hit;

const int spheres_size = 1;
Sphere spheres[spheres_size] = Sphere[spheres_size] (Sphere(vec3(0.0, 0.0, 5), 2.0, vec4(1.0, 0.0, 0.5, 1.0)));

const int planes_size = 1;
Plane planes[planes_size] = Plane[planes_size] (Plane(vec3(0.0, 1.0, 0.0), vec3(0.0, -1.0, 0.0), vec4(0.25, 0.25, 0.25, 1.0)));

vec3 sun = vec3(-1.5, 1.0, 0.0);
const vec4 ambient = vec4(0.25);

vec4 normal_to_rgb(vec3 v);
vec3 sceneSDF(vec3 p);
Hit march(vec3 o, vec3 r);
vec3 estimateNormal(vec3 p);
float AmbientOcclusion(vec3 point, vec3 normal, float step_dist, float step_nbr);
float sdf_sphere(vec3 p, int i);
float sdf_plane(vec3 p, int i);
float DE(vec3 pos);
vec4 get_color(Hit m);
vec4 gamma_correct(vec4 c);

void main() {
    sun = vec3(sin(t) * -1.5, 1.0, cos(t));
    float rand = 0.0001;
    vec3 ray = normalize(vec3((fragTexCoord.x * 2.0) - 1.0, (fragTexCoord.y * 2.0) - 1.0, 2.0));
    Hit m = march(vec3(0.0, 0.0, -2.25), ray);
    vec4 color = gamma_correct(get_color(m));
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
    /*
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
    */
    d = DE(p);
    min = float(d < min) * d + float(min <= d) * min;
    type = int(d < EPSILON) * SCENE_MANDEL + int(EPSILON <= d) * type;
    
    return vec3(min, last_i, type);
}

Hit march(vec3 o, vec3 r) {
    float d = 1;
    for(int i = 0; i < MAX_MARCH_STEPS; i++) {
        vec3 sdf = sceneSDF(o + (d * r));
        float dist = sdf.x;
        if(dist < EPSILON) {
            return Hit(d, i, int(sdf.y), int(sdf.z), o + d * r, estimateNormal(o + d * r));
        }
        d += dist;
        if(d >= FARPLANE) {
            return Hit(FARPLANE, i, -1, SCENE_INVALID, o + FARPLANE * r, estimateNormal(o + FARPLANE * r));
        }
    }
    return Hit(FARPLANE, -1, MAX_MARCH_STEPS - 1, SCENE_INVALID, o + FARPLANE * r, estimateNormal(o + FARPLANE * r));
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

float DE(vec3 pos) {
    const int Iterations = 10;
    const float Bailout = 1.25;
    const float Power = 4;
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;
    for(int i = 0; i < Iterations; i++) {
        r = length(z);
        if(r > Bailout)
            break;

		// convert to polar coordinates
        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
        dr = pow(r, Power - 1.0) * Power * dr + 1.0;

		// scale and rotate the point
        float zr = pow(r, Power);
        theta = theta * Power;
        phi = phi * Power;

		// convert back to cartesian coordinates
        z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
        z += pos;
    }
    return 0.5 * log(r) * r / dr;
}

float AmbientOcclusion(vec3 point, vec3 normal, float step_dist, float step_nbr) {
    float occlusion = 1.0f;
    while(step_nbr > 0.0) {
        occlusion -= pow(step_nbr * step_dist - (sceneSDF(point + normal * step_nbr * step_dist)).x, 2) / step_nbr;
        step_nbr--;
    }

    return occlusion;
}

vec4 gamma_correct(vec4 c) {
    return c;//vec4(pow(c.xyz, vec3(1.0 / 2.2)), 1.0);
}

vec4 get_color(Hit m) {
    vec4 obj_color = vec4(0.9, 0, 1, 1);
    float s = min(max(dot(m.normal, sun), 0), 1.0);
    float ambient_occlusion = min(pow(AmbientOcclusion(m.point, m.normal, 0.015, 20), 40), 1.0);
    switch(m.type) {
        case SCENE_INVALID:
            obj_color = vec4(0.25);
            break;
        case SCENE_SPHERE:
            obj_color = spheres[m.index].c;
            break;
        case SCENE_PLANE:
            obj_color = planes[m.index].c;
            break;
        case SCENE_MANDEL:
            obj_color = vec4(0.5, 0.26, 0.5, 1.0);
            break;
    }
    return m.d < FARPLANE ? (obj_color * s + ambient) * ambient_occlusion : vec4(0.0, 0.0, 0.0, 1.0);
}