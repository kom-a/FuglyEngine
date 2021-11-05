#version 330 core

out vec4 FragColor;

in VS_OUT
{
	vec4 v_FragPos;
	vec3 v_Normal;
	vec2 v_TexCoords;
	vec3 v_Color;
} fs_in;

uniform vec3 u_CameraPos;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

void main()
{
	vec3 lightPos = vec3(-1000, 1000, 1000);
	vec3 lightDir = normalize(lightPos - fs_in.v_FragPos.xyz);
	vec3 viewDir = normalize(u_CameraPos - fs_in.v_FragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, fs_in.v_Normal);

	vec3 diffuseColor = texture(diffuseSampler, fs_in.v_TexCoords).xyz * fs_in.v_Color;
	vec3 specularColor = texture(specularSampler, fs_in.v_TexCoords).xyz;

	vec3 ambient = diffuseColor * 0.05f;
	vec3 diffuse = diffuseColor * max(0.0f, dot(fs_in.v_Normal, lightDir));
	vec3 specular = specularColor * pow(max(dot(viewDir, reflectDir), 0.0), 64);
	
	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
}