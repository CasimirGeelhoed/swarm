#version 150 core

uniform UBO
{
    uniform vec3 color;
} ubo;


out vec4 out_Color;
in vec3 pass_Position;

void main()
{
	float dist = length(pass_Position - vec3(0, 0, 0));
    out_Color = vec4(ubo.color, 0.5 - dist);
}
