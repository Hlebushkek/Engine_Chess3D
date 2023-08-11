#version 410 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    sampler2D diffuseTex;
    sampler2D specularTex;
};

struct Light
{
    int type;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

in vec4 vs_color;
in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform Light lights[20];

uniform vec3 cameraPos;
uniform bool isTextureBound;

vec3 CalcPointLight(Light light, vec3 vs_position, vec3 vs_normal, vec2 vs_texcoord, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(light.position - vs_position);
    // diffuse shading
    float diff = max(dot(vs_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, vs_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), /*material.shininess*/ 2);
    // attenuation
    float distance = length(light.position - vs_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseTex, vs_texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, vs_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularTex, vs_texcoord));
    // apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(Light light, vec3 vs_position, vec3 vs_normal, vec2 vs_texcoord, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(vs_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, vs_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), /*material.shininess*/ 2);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseTex, vs_texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, vs_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularTex, vs_texcoord));

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 vs_position, vec3 vs_normal, vec2 vs_texcoord, vec3 viewDir, Material material)
{
    vec3 lightDir = normalize(light.position - vs_position);
    // diffuse shading
    float diff = max(dot(vs_normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, vs_normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), /*material.shininess*/ 2);
    // attenuation
    float distance = length(light.position - vs_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuseTex, vs_texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTex, vs_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularTex, vs_texcoord));
    // apply attenuation & intensity
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

void main()
{
    if (isTextureBound)
    {
        vec3 viewDir = normalize(cameraPos - vs_position);

        vec3 result = vec3(0.f);
        for (int i = 0; i < 20; i++)
        {
            switch (lights[i].type)
            {
            case 1:
                result += CalcPointLight(lights[i], vs_position, normalize(vs_normal), vs_texcoord, viewDir, material);
                break;
            case 2:
                result += CalcDirLight(lights[i], vs_position, normalize(vs_normal), vs_texcoord, viewDir, material);
                break;
            case 3:
                result += CalcSpotLight(lights[i], vs_position, normalize(vs_normal), vs_texcoord, viewDir, material);
                break;
            default:
                break;
            }
        }

        // fs_color = texture(material.diffuseTex, vs_texcoord) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(1, 1, 1, 1));
        fs_color = texture(material.diffuseTex, vs_texcoord) * vec4(result, 1.f);
    } else {
        fs_color = vs_color;
    }
}