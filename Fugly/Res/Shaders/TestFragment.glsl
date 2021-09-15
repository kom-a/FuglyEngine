#version 330 core

out vec4 FragColor;

in vec4 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoords;

uniform vec3 u_CameraPos;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;
 
float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	vec3 lightPos = vec3(-1000, 1000, 1000);
	vec3 lightDir = normalize(lightPos - v_FragPos.xyz);
	vec3 viewDir = normalize(u_CameraPos - v_FragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, v_Normal);

	vec3 diffuseColor = texture(diffuseSampler, v_TexCoords).xyz;
	vec3 specularColor = texture(specularSampler, v_TexCoords).xyz;

	vec3 ambient = diffuseColor * 0.1f;
	vec3 diffuse = diffuseColor * max(0.0f, dot(v_Normal, lightDir));
	vec3 specular = specularColor * pow(max(dot(viewDir, reflectDir), 0.0), 64);
	
	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
	FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0f);
}