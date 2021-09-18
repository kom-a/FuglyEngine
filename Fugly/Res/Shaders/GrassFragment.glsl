#version 330 core

out vec4 FragColor;

in vec2 v_TexCoords;

uniform sampler2D sampler;

void main()
{
	FragColor = texture(sampler, v_TexCoords);
}