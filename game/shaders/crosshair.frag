#version 460

in vec2 v_TextureCoordinates;

uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, v_TextureCoordinates);
}
