#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;
out vec3 normal;

void main() {
	vec3 light_position = vec3(0, 1, 1);
	int	material_shininess = 70;
	float material_kd = 0.7;
	float material_ks = 0.7;
	vec3 eye_position = vec3(0, 2, 5);
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;

	vec3 L = normalize(light_position - world_pos);
	vec3 N = normalize((Model * vec4(v_normal, 0)).xyz);
	vec3 R = normalize(reflect(-L, N));
	vec3 V = normalize(eye_position - world_pos);

	float ambient_light = 0.5;
	float diffuse_light = dot(N, L);
	float specular_light = 0;

	if (diffuse_light > 0) {
		specular_light = pow(max(0, dot(R, V)), material_shininess);
	}
	
	float d	= distance(light_position, v_position);
	float attenuation_factor = 1 / (0.05 * d * d + 1);
	float light = ambient_light + attenuation_factor * (max(0, diffuse_light) * material_kd + specular_light * material_ks);

	color = light * object_color;
	normal = v_normal;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
