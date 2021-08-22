#version 330 core

out vec4 FragColor;

in vec4 v_Color;
in vec4 v_Position;

uniform vec2 u_LightPos;

void main()
{
	float intensity = 0.2f / length(u_LightPos - v_Position.xy);

	FragColor = v_Color;
}