#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 position;
layout(location = 3) in vec3 normal;
layout(location = 2) in vec2 texture; 
layout(location = 1) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 pozCar;

out vec3 frag_color;
out vec3 frag_normal;
out vec2 texcoord;

// Output
// TODO(student): Output values to fragment shader

void main()
{
    texcoord = texture;
    float scaleFactor =  0.02;
    // TODO(student): Send output to fragment shader
    frag_color = color;
    // TODO(student): Compute gl_Position
    vec3 pozW = vec3(Model[3]) + position;
    float Posy = pozW.y - (length(pozCar - pozW)*length(pozCar - pozW) * scaleFactor);
    vec3 newpoz = vec3(position.x, Posy, position.z);
    gl_Position = Projection * View * Model * vec4(newpoz, 1);
}
