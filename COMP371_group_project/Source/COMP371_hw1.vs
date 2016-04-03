#version 130

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform int drawType;
uniform vec3 camPos;

in  vec3 in_Position;		//vertex position
out vec3 out_Color;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM * vec4 (in_Position, 1.0);

	out_Color = vec3 (in_Position.x,in_Position.y,in_Position.z);

	if(drawType == 1){// street grey
		out_Color = vec3 (0.54f, 0.54f, 0.54f);
		//out_Color = vec3 (camPos.x, camPos.y, camPos.z);
	}
	if(drawType == 2){// street WHITE
		out_Color = vec3 (0.0f, 0.0f, 0.0f);
	}

	vec3 distanceFromCamToPos = in_Position - camPos;
	float distance = distanceFromCamToPos.length();



}