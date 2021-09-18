#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Projection = mat4(1.0f);
uniform mat3 u_NormalMatrix = mat3(1.0f);

void main()
{
	v_TexCoords = a_TexCoords;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0f);
}