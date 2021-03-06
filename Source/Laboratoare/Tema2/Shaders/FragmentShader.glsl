#version 330

uniform sampler2D texture_1;

// Get color value from vertex shader
in vec3 color;
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// Write pixel out color
	//out_color = vec4(color, 1);


	vec4 color = texture2D(texture_1, texcoord);
	out_color = color;
}