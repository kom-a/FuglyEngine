#version 330 core

out vec4 FragColor;

in vec4 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform vec3 u_CameraPos;

uniform sampler2D sampler;

void main()
{
	vec3 lightPos = vec3(-1000, 1000, 1000);
	vec3 lightDir = normalize(lightPos - v_FragPos.xyz);
	vec3 viewDir = normalize(u_CameraPos - v_FragPos.xyz );
	vec3 reflectDir = reflect(-lightDir, v_Normal);

	float ambient = 0.1f;
	float diffuse = max(0.0f, dot(v_Normal, lightDir));
	float specularStrenth = 0.5f;
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 64);

	vec3 color = texture(sampler, v_TexCoords).xyz;
	// vec3 color = vec3(0.8f, 0.5f, 0.3f);

	vec3 result = (ambient + diffuse + specular) * color;

	FragColor = vec4(result, 1.0f);
}