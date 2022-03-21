#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
// layout (location = 2) in vec4 a_Texture;

uniform mat4 u_ProjMatrix;
uniform mat4 u_ViewMatrix;

out vec3 v_Color;

void main()
{
    // gl_Position = vec4(a_Position, 1.0);
    gl_Position = u_ProjMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
    v_Color = a_Color;
}