#version 330 core

in vec4 vertexColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float alpha;

void main()
{
    FragColor = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), alpha);
} 
