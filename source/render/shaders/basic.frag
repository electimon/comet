#version 460

in vec3 v_Position;
in vec2 v_TextureCoordinates;
in vec3 v_Normal;
in float v_AO;
in float v_Brightness;

in float v_Time;

uniform sampler2D u_TextureMap;

out vec4 FragColor;

void main()
{
    // vec3 lightPos = vec3(0.0, 100.0, 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // vec3 objectColor = vec3(v_Color.x, v_Color.y, v_Color.z);
    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;
    // diffuse
    vec3 norm = normalize(v_Normal);
    // vec3 lightDir = normalize(lightPos - v_Position);
    vec3 lightDir = vec3(0.5, 1.0, 0.75);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // vec3 result = vec3(v_Color.x, v_Color.y, v_Color.z) * v_AO;

    float dayLength = 1.0;
    // float brightness = max(cos(dayLength * v_Time), 0.3);



    vec4 result = vec4((ambient + diffuse) / 2.0, 1.0);
    vec4 brightness = vec4(1.0, 1.0, 1.0, v_Brightness);

    FragColor = texture(u_TextureMap, v_TextureCoordinates) * result * brightness;
}