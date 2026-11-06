
// shadow map(depth)
GLSL_SHADER const char VK_DEPTH_FRAG[] =
"#version 450\n"
"#pragma shader_stage( fragment )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"#ifdef _DEBUG\n"
"layout(location = 0) out vec4 _gl_FragColor;\n"
"#endif\n"
"\n"
"void main(void)\n"
"{\n"
"#ifdef _DEBUG\n"
"    _gl_FragColor = vec4((gl_FragCoord.z + 1.0) * 0.5, 0.0, 0.0, 1.0); // DEBUG\n"
"#endif\n"
"}\n"
;
