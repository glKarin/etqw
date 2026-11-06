
// fog
GLSL_SHADER const char VK_FOG_FRAG[] =
"#version 450\n"
"#pragma shader_stage( fragment )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(location = 0) in vec2 var_TexFog;            // input Fog TexCoord\n"
"layout(location = 1) in vec2 var_TexFogEnter;       // input FogEnter TexCoord\n"
"\n"
"layout(binding = 1) uniform UniformBufferObject {\n"
"    vec4 u_fogColor;       // Fog Color\n"
"};\n"
"\n"
"layout(binding = 2) uniform sampler2D u_fragmentMap0;   // Fog Image\n"
"layout(binding = 3) uniform sampler2D u_fragmentMap1;   // Fog Enter Image\n"
"\n"
"layout(location = 0) out vec4 _gl_FragColor;\n"
"\n"
"void main(void)\n"
"{\n"
"    _gl_FragColor = texture( u_fragmentMap0, var_TexFog ) * texture( u_fragmentMap1, var_TexFogEnter ) * vec4(u_fogColor.rgb, 1.0);\n"
"}\n"
;
