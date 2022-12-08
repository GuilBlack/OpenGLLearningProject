#shader vertex
#version 420

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 v_Normal;
out vec3 v_CurrentPos;
out vec2 v_Uv;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(pos, 1.0f);
	v_CurrentPos = vec3(u_Model * vec4(pos, 1.f));
	v_Normal = normal;
	v_Uv = uv;
}

#shader fragment
#version 420

out vec4 color;

in vec3 v_CurrentPos;
in vec3 v_Normal;
in vec2 v_Uv;

uniform sampler2D tex;
uniform vec3 u_CamPos;
struct PointLight
{
	vec3 Pos;
	vec3 Color;
};
uniform PointLight u_PointLight;

void main()
{
	float ambient = 0.1f;
	vec3 lightDirection = normalize(u_PointLight.Pos - v_CurrentPos);
	float diffuse = max(dot(v_Normal, lightDirection), 0.0f);

	float specularLight = 1;
	float specular = 0.f;
	if (diffuse != 0.f)
	{
		vec3 viewDir = normalize(u_CamPos - v_CurrentPos);
		vec3 halfwayVec = normalize(lightDirection + viewDir);
		float specAmount = pow(max(dot(v_Normal, halfwayVec), 0.0f), 8);
		specular = specularLight * specAmount;
	}

	color = texture(tex, v_Uv) * vec4(u_PointLight.Color * (diffuse + ambient + specular), 1.0f);
}