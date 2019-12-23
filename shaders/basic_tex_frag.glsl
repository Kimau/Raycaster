#version 330

uniform sampler2D LTextureUnit;
in vec4 Frag_Color;
in vec2 Tex_UV;
out vec4 Out_Color;

void main()
{
	Out_Color = texture( LTextureUnit, Tex_UV ) * Frag_Color; 
}
