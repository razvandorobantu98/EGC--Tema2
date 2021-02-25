#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: write pixel out color
	out_color = vec4(object_color, 1);
}