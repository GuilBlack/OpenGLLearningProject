#shader vertex
#version 420

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 col;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 vCol;
out vec2 vUv;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	vCol = col;
	vUv = uv;
}

#shader fragment
#version 420

out vec4 color;

uniform sampler2D tex;

in vec4 vCol;
in vec2 vUv;

void main()
{
	color = texture(tex, vUv);
}