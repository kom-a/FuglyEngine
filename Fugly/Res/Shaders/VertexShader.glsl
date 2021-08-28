#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;
out vec4 v_Position;

uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Projection = mat4(1.0f);

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
	v_Color = a_Color;
	v_Position = u_Model * vec4(a_Pos, 1.0f);
}