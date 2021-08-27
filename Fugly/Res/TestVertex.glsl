#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMatrix;

out vec4 v_FragPos;
out vec3 v_Normal;

void main()
{
	v_FragPos = u_Model * vec4(a_Pos, 1.0f);
	v_Normal = normalize(u_NormalMatrix * a_Normal);
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}
