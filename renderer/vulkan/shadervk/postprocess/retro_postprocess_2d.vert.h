
// retro postprocess 2D general vertex shader
GLSL_SHADER const char VK_RETRO_POSTPROCESS_2D_VERT[] =
"#version 450\n"
"#pragma shader_stage( vertex )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(location = 8) in vec4 attr_TexCoord;\n"
"layout(location = 0) in vec4 attr_Vertex;\n"
"\n"
"layout(location = 0) out vec2 var_TexCoord;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_Position = attr_Vertex;\n"
"    gl_Position.y = -attr_Vertex.y;\n"
"\n"
"    var_TexCoord = attr_TexCoord.xy;\n"
"}\n"
;
