#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 position;
uniform mat4 transform;

void main()
{
  gl_Position = vec4(aPos, 0.0, 1.0)*transform + vec4(position, 0.0, 0.0);
}