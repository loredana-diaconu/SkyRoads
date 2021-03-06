#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;
uniform float time;
uniform int power;

// Output value to fragment shader
out vec3 color;
out vec3 normal;
out vec2 texcoord;

const float amplitude =1;
const float frequency = 4;
const float PI = 3.14159;

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

// Noise function https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float noise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res;
}

void main() {
	vec3 light_position = vec3(0, 1, 1);
	int	material_shininess = 30;
	float material_kd = 0.5;
	float material_ks = 0.5;
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
	texcoord = v_texcoord;

	float disp = noise(vec2(v_normal.x * time * 0.5, v_normal.y)) - 0.3;
	vec3 newPos = v_position + v_normal * disp * cos(3 * time);

	if (power == 1) {
		gl_Position = Projection * View * Model * vec4(newPos, 1.0);
	} else {
		gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	}	
} 

