
// shadow
GLSL_SHADER const char VK_SHADOW_FRAG[] =
"#version 450\n"
"#pragma shader_stage( fragment )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(location = 0) in vec4 var_Color;\n"
"\n"
"layout(location = 0) out vec4 _gl_FragColor;\n"
"\n"
"void main(void)\n"
"{\n"
"    _gl_FragColor = var_Color;\n"
"}\n"
;
