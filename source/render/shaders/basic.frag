#version 460

in vec3 v_Position;
in vec4 v_Color;
in vec3 v_Normal;
in float v_AO;

in float v_Time;

out vec4 FragColor;

void main()
{
    // vec3 lightPos = vec3(0.0, 100.0, 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 objectColor = vec3(v_Color.x, v_Color.y, v_Color.z);

    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(v_Normal);
    // vec3 lightDir = normalize(lightPos - v_Position);
    vec3 lightDir = vec3(0.5, 1.0, 0.75);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) / 2 * objectColor * v_AO;
    // vec3 result = vec3(v_Color.x, v_Color.y, v_Color.z) * v_AO;
    FragColor = vec4(result, 1.0);
}