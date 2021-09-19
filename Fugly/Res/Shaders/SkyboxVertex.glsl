#version 330 core

layout (location = 0) in vec3 a_Pos;

out vec3 v_TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	v_TexCoords = a_Pos;
	gl_Position = u_Projection * u_View * vec4(a_Pos, 1.0f);
}