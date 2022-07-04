#shader vertex
#version 420

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;

out vec4 vCol;

void main()
{
	gl_Position = projection * model * vec4(pos, 1.0f);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}

#shader fragment
#version 420

in vec4 vCol;
out vec4 colour;

void main()
{
	colour = vCol;
}