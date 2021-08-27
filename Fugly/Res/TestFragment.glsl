#version 330 core

out vec4 FragColor;

in vec4 v_FragPos;
in vec3 v_Normal;

void main()
{
	vec3 lightPos = vec3(-1000, 1000, 1000);
	vec3 lightDir = normalize(lightPos - v_FragPos.xyz);

	vec3 ambient = vec3(0.1);

	float diffuse = max(0.0f, dot(v_Normal, lightDir));

	vec3 color = vec3(0.8f, 0.5f, 0.3f);

	vec3 result = (ambient + diffuse) * color;

	FragColor = vec4(result, 1.0f);
}