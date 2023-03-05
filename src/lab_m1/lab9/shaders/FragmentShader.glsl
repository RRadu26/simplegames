#version 330

// Input
in vec2 texcoord;
uniform int t2;
// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
	vec4 color;
	vec4 color1 = texture2D(texture_1, texcoord); 
	vec4 color2 = texture2D(texture_2, texcoord);
	if(t2 == 1)
		color = mix(color1, color2, 0.5f);
	else
		color = color1;
    if(color.q < 0.5)
		discard;
	out_color = color;

}
