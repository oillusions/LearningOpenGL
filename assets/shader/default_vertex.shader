#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 tranMatrix;

out vec3 Color;
out vec2 TexCoord;

void main() {

	gl_Position = tranMatrix *vec4(aPos, 1.0f);
	Color = aColor;
	TexCoord = aTexCoord;
}
