#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;
layout(location = 3) in vec3 a_Color;

uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Projection = mat4(1.0f);
uniform mat3 u_NormalMatrix = mat3(1.0f);

out vec4 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoords;
out vec3 v_Color;

void main()
{
	v_FragPos = u_Model * vec4(a_Pos, 1.0f);
	v_Normal = normalize(u_NormalMatrix * a_Normal);
	v_TexCoords = a_TexCoords;
	v_Color = a_Color;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0f);
}
