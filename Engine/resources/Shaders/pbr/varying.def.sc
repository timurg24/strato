// vertex attributes
vec3 a_position     : POSITION;
vec3 a_normal       : NORMAL;
vec2 a_texcoord0    : TEXCOORD0;

// vertex -> fragment
vec3 v_fragPos      : TEXCOORD0 = vec3(0.0, 0.0, 0.0);
vec3 v_normal       : TEXCOORD1 = vec3(0.0, 0.0, 0.0);
vec2 v_texCoord     : TEXCOORD2 = vec2(0.0, 0.0);