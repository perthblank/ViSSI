#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexPosition_modelspace_2d;
layout(location = 2) in float vertexValue;
layout(location = 10) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 MVP;
uniform int draw_flag;
uniform int color_flag;
uniform float gb_value;
uniform float scale;

void main(){	

	if(color_flag == 0)
	{
		fragmentColor = vertexColor;
	}
	else if(color_flag == 1)
	{ //black
		fragmentColor = vec3(0.0,0,0);
	}
	else if(color_flag == 2)
	{ //red
		fragmentColor = vec3(1.0,0,0);
	}
	else if(color_flag == 3)
	{ //blue
		fragmentColor = vec3(0.0,0,1.0);
	}
	else if(color_flag ==10)
	{ //value-diff-based mode
		float diff = abs(vertexValue-gb_value)/scale;
		fragmentColor = vec3(1-diff, diff,0 );
	}
		
	if(draw_flag == 1){
		// draw 3D vertex
		gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
	}
	else if(draw_flag == 2){
		// draw 2D vertex on x-z
		gl_Position =  MVP * vec4(vertexPosition_modelspace_2d.x, 0, vertexPosition_modelspace_2d.y, 1);
	}
	else if(draw_flag == 20){
		// draw value on y
		gl_Position =  MVP * vec4(vertexPosition_modelspace_2d.x, vertexValue, vertexPosition_modelspace_2d.y, 1);

	}

	

}

