#version 330

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform int drawType;

layout(location = 0) in vec3 vertexPosition;
layout(location = 3) in vec3 vertexColor;

out vec3 frag_Color;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM * vec4 (vertexPosition, 1.0);

	frag_Color = vertexColor;
}