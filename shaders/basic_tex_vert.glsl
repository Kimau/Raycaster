#version 330

in vec4 Position;
in vec4 Color;
in vec2 TexCoords;
out vec4 Frag_Color;
out vec2 Tex_UV;
uniform mat4 ProjMtx;

void main()
{
	Frag_Color = Color;
	Tex_UV = TexCoords;
	gl_Position = ProjMtx * Position;
}
