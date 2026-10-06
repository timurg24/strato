$input v_fragPos, v_normal, v_texCoord

#include <bgfx_shader.sh>

uniform vec4 u_baseColor;

SAMPLER2D(u_textureAlbedo, 0);

// Sun
uniform vec4 sunDirection; // xyz = direction, w = light count
uniform vec4 sunColor;     // rgb = color, w = intensity

void main()
{
    vec4 albedoTexture =
        texture2D(u_textureAlbedo, v_texCoord);

    vec3 albedo =
        albedoTexture.rgb * u_baseColor.rgb;

    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(-sunDirection.xyz);

    float sunIntensity = sunColor.w;

    // Ambient
    float ambientStrength = 0.1;

    vec3 ambient =
        ambientStrength * sunColor.rgb * sunIntensity;

    // Diffuse
    float diff =
        max(dot(norm, lightDir), 0.0);

    vec3 diffuse =
        diff * sunColor.rgb * sunIntensity;

    // Final color
    vec3 result =
        (ambient + diffuse) * albedo;

    gl_FragColor =
        vec4(result, albedoTexture.a);
}