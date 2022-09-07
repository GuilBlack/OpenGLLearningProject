#shader vertex
#version 420

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 col;

uniform mat4 model;
uniform mat4 projection;

out vec4 vCol;

void main()
{
	gl_Position = projection * model * vec4(pos, 1.0f);
	vCol = col;
}

#shader fragment
#version 420

in vec4 vCol;
out vec4 colour;

void main()
{
	colour = vCol;
}