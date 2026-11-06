
// cubemap
GLSL_SHADER const char VK_CUBEMAP_FRAG[] =
"#version 450\n"
"#pragma shader_stage( fragment )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(location = 0) in vec3 var_TexCoord;\n"
"layout(location = 1) in vec4 var_Color;\n"
"\n"
"layout(binding = 2) uniform samplerCube u_fragmentCubeMap0;\n"
"\n"
"layout(binding = 1) uniform UniformBufferObject {\n"
"    vec4 u_glColor;\n"
"};\n"
"\n"
"layout(location = 0) out vec4 _gl_FragColor;\n"
"\n"
"void main(void)\n"
"{\n"
"    _gl_FragColor = texture(u_fragmentCubeMap0, var_TexCoord) * u_glColor * var_Color;\n"
"}\n"
;
