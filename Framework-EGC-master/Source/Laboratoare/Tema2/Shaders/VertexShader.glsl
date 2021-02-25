#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;

uniform float timeElapsed;
uniform bool isCylinder;

void main()
{
	// TODO: compute world space vertex position and normal
	vec3 world_pos = (Model * vec4(v_position,1)).xyz;
	vec3 world_norm = normalize( mat3(Model) * v_normal );

	// TODO: send world position and world normal to Fragment Shader
	world_position = world_pos;
	world_normal = world_norm;

	vec3 pos = v_position;

	//daca este un varf al cilindrului, roteste varful in functie de pozitia initiala (efect de val)
	if(isCylinder == true) {
		gl_Position = Projection * View * Model * vec4(pos.x, pos.y, pos.z  + sin(pos.z * 20 + timeElapsed/10 * 20) * 0.1, 1.0);
	}
	else {
		gl_Position = Projection * View * Model * vec4(pos, 1.0);
	}
}
