#version 130

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform int drawType;
uniform vec3 camPos;

in  vec3 in_Position;		//vertex position
out vec3 out_Color;
out float distance;
out vec4 gl_Position;

void main () {
	mat4 CTM = proj_matrix * view_matrix * model_matrix;
	gl_Position = CTM * vec4 (in_Position, 1.0);

	// based on position
	//out_Color = vec3 (in_Position.x,in_Position.y,in_Position.z);

	vec3 distanceFromCamToPos = vec3 (in_Position.x, in_Position.y, in_Position.z) - camPos;

	if(drawType == 0){// BUILDING
		out_Color = vec3 (1.0f, 0.0f, 0.0f);//red
		distance = sqrt((distanceFromCamToPos.x * distanceFromCamToPos.x) + (distanceFromCamToPos.z * distanceFromCamToPos.z));
	}

	if(drawType == 2){// WORLD PLANE
		out_Color = vec3 (0.0f, 1.0f, 0.0f);//green
		distance = sqrt((distanceFromCamToPos.x * distanceFromCamToPos.x) +  (distanceFromCamToPos.z * distanceFromCamToPos.z));
	}
	if(drawType == 3){// STREET
		out_Color = vec3 (0.0f, 0.0f, 1.0f);//blue
		distance = sqrt((distanceFromCamToPos.x * distanceFromCamToPos.x) +  (distanceFromCamToPos.z * distanceFromCamToPos.z));
	}

}