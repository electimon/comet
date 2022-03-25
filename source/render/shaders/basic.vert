#version 460

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TextureCoordinates;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in float a_AO;

uniform mat4 u_ProjMatrix;
uniform mat4 u_ViewMatrix;
uniform float u_Time;

out vec3 v_Position;
out vec2 v_TextureCoordinates;
out vec3 v_Normal;
out float v_AO;
out float v_Time;

void main()
{
    gl_Position = u_ProjMatrix * u_ViewMatrix * vec4(a_Position, 1.0);

    v_Position = a_Position;
    v_TextureCoordinates = a_TextureCoordinates;
    v_Normal = a_Normal;
    v_AO = a_AO;

    v_Time = u_Time;
}