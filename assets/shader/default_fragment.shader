#version 330 core

in vec3 Color;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D wall_texture;
uniform sampler2D awesomeface_texture;

void main() {
	FragColor = mix(texture(wall_texture, TexCoord), texture(awesomeface_texture, TexCoord), 0.5f);
}