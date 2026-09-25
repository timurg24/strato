$input v_fragPos, v_normal, v_texCoord

#include <bgfx_shader.sh>

uniform vec4 u_baseColor;

// Material texture
SAMPLER2D(u_textureAlbedo, 0);

// sun
uniform vec4 sunDirection;
uniform vec4 sunAmbient;
uniform vec4 sunDiffuse;
uniform vec4 sunSpecular;

void main()
{
    // Read albedo texture
    vec4 albedoTexture =
        texture2D(u_textureAlbedo, v_texCoord);

    // Combine texture color with material base color
    vec3 albedo =
        albedoTexture.rgb * u_baseColor.rgb;

    // ambient
    float ambientStrength = 0.1;

    vec3 ambient =
        ambientStrength * sunAmbient.xyz;

    // diffuse
    vec3 norm =
        normalize(v_normal);

    float diff =
        max(
            dot(norm, sunDirection.xyz),
            0.0
        );

    vec3 diffuse =
        diff * sunDiffuse.xyz;

    // final color
    vec3 result =
        (ambient + diffuse) * albedo;

    gl_FragColor =
        vec4(result, albedoTexture.a);
}