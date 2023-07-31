#version 410 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    sampler2D diffuseTex;
    sampler2D specularTex;
};

in vec4 vs_color;
in vec3 vs_position;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform vec3 lightPos0;
uniform vec3 cameraPos;
uniform bool isTextureBound;

vec3 calcAmbient(Material material)
{
    return material.ambient;
}

vec3 calcDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
    vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
    float diffuseConst = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
    vec3 diffuseFinal = material.diffuse * diffuseConst;

    return diffuseFinal;
}

vec3 calcSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
    vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
    vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
    vec3 posToViewDirVec = normalize(cameraPos - vs_position);
    float specularConst = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 20);
    vec3 specularFinal = material.specular * specularConst * texture(material.specularTex, vs_texcoord).rgb;

    return specularFinal;
}

void main()
{
    //Ambient light
    vec3 ambientFinal = calcAmbient(material);

    //Diffuse light
    vec3 diffuseFinal = calcDiffuse(material, vs_position, vs_normal, lightPos0);

    //Specular ligt
    vec3 specularFinal = calcSpecular(material, vs_position, vs_normal, lightPos0, cameraPos);

    //Attenuation


    //Final light
    if (isTextureBound)
    {
        // fs_color = texture(material.diffuseTex, vs_texcoord) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(1, 1, 1, 1));
        fs_color = texture(material.diffuseTex, vs_texcoord) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
    } else {
        fs_color = vs_color;
    }
}