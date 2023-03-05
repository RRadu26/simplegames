#version 330

// Input
uniform sampler2D texture_1;

// Output
layout(location = 0) out vec4 out_color;
in vec3 frag_color;
in vec3 frag_normal;
in vec2 texcoord;
void main() {
	vec4 color = texture2D(texture_1, texcoord);         
	out_color = color;
}
