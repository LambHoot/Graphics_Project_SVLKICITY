#version 330 core

uniform mat4 view_matrix, model_matrix, proj_matrix;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 vertexColor;

out vec3 world_pos;
out vec3 world_normal;
out vec4 viewSpace;

out vec3 frag_Color;

void main () {
	//used for lighting models
	
	world_pos = (model_matrix * vec4(vertexPosition, 1.0f)).xyz;
	world_normal = normalize(mat3(model_matrix) * in_normal);

	viewSpace = view_matrix * model_matrix * vec4(vertexPosition,1);
	//mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = proj_matrix * viewSpace;

	frag_Color = vertexColor;
}


 