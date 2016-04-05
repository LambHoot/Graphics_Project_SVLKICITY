#version 330

uniform mat4 view_matrix, model_matrix, proj_matrix;
uniform int drawType;
uniform vec3 camPos;

in  vec3 in_Position;		//vertex position
out vec3 out_Color;
out float distance;

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
	distance = sqrt((distanceFromCamToPos.x * distanceFromCamToPos.x) + (distanceFromCamToPos.y * distanceFromCamToPos.y) + (distanceFromCamToPos.z * distanceFromCamToPos.z));

	float f = (1.0f-((100.0f - distance)/100.0f));
	vec3 fogColor = vec3 (1.0f, 1.0f, 1.0f);
	float ourColorR = (f*fogColor.x) + (1-f)*out_Color.x;
	float ourColorG = (f*fogColor.y) + (1-f)*out_Color.y;
	float ourColorB = (f*fogColor.z) + (1-f)*out_Color.z;
	vec3 wow = vec3 (ourColorR, ourColorG, ourColorB);
	out_Color += wow;
	if(distance > 200.0f){
		out_Color = vec3 (1.0f,1.0f,1.0f);
	}

	//out_Color = distanceFromCamToPos;

}