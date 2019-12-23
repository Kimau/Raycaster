#version 330

in vec4 Position;
in vec4 Color;
out vec4 Frag_Color;
uniform mat4 ProjMtx;

void main()
{
	Frag_Color = Color;
	gl_Position = ProjMtx * Position;
}