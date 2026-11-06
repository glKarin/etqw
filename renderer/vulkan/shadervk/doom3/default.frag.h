
// diffuse map
GLSL_SHADER const char VK_DEFAULT_FRAG[] =
"#version 450\n"
"#pragma shader_stage( fragment )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(binding = 1) uniform UniformBufferObject {\n"
"    vec4 u_glColor;\n"
"};\n"
"\n"
"layout(binding = 2) uniform sampler2D u_fragmentMap0;\n"
"\n"
"layout(location = 0) in vec2 var_TexDiffuse;\n"
"layout(location = 1) in vec4 var_Color;\n"
"\n"
"layout(location = 0) out vec4 _gl_FragColor;\n"
"\n"
"void main(void)\n"
"{\n"
"    _gl_FragColor = texture(u_fragmentMap0, var_TexDiffuse) * u_glColor * var_Color;\n"
"}\n"
;
