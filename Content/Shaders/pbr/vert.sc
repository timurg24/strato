$input a_position, a_normal, a_texcoord0
$output v_fragPos, v_normal, v_texCoord

#include <bgfx_shader.sh>

void main()
{
    vec4 worldPos = mul(
        u_model[0],
        vec4(a_position, 1.0)
    );

    v_fragPos = worldPos.xyz;

    v_normal = normalize(
        mul(
            u_model[0],
            vec4(a_normal, 0.0)
        ).xyz
    );

    v_texCoord = a_texcoord0;

    gl_Position =
        mul(
            u_modelViewProj,
            vec4(a_position, 1.0)
        );
}