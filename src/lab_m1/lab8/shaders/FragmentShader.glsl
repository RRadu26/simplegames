#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_positions[3];
uniform vec3 eye_position;
uniform float angle;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int spotlight;
// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    float k = 0;
    float ambient_light = 0.25;

    for(int i = 0 ; i < 3; i++) {
    vec3 worldnor = normalize(world_normal);
    vec3 L = normalize( light_positions[i] - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 R = reflect (-L, worldnor);
    // TODO(student): Define ambient, diffuse and specular light components
    int primlum = 0;
    float diffuse_light = material_kd  * max(dot(worldnor,L), 0);


    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0) {
        primlum = 1;
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float specular_light = material_ks * primlum * pow(max(dot(V,R), 0), 5);
    float kc = 1, kl = 0, kq = 1;
    float d = distance(light_positions[i], world_position);
    float att = 1/(kc + kl + kq * d*d);
    if(spotlight == 1) {
        float cut_off = radians(angle);
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);
        diffuse_light *= light_att_factor;
        specular_light *= light_att_factor;
        if(cos(cut_off) > spot_light) {
            diffuse_light = 0;
            specular_light = 0;
        }
    }
    
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
    k += att*specular_light + att*diffuse_light;
    }
    out_color = vec4(object_color * (ambient_light + k), 1);

}
