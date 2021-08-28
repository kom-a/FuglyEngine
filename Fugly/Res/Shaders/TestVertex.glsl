#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMatrix;

out vec4 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;

void main()
{
	v_FragPos = u_Model * vec4(a_Pos, 1.0f);
	v_Normal = normalize(u_NormalMatrix * a_Normal);
	v_TexCoords = a_TexCoords;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}
