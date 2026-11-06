
// z-fill perforated depth
GLSL_SHADER const char VK_DEPTH_PERFORATED_VERT[] =
"#version 450\n"
"#pragma shader_stage( vertex )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(location = 8) in vec4 attr_TexCoord;\n"
"layout(location = 0) in vec4 attr_Vertex;\n"
"\n"
"layout(binding = 0) uniform UniformBufferObject {\n"
"    mat4 u_textureMatrix;\n"
"    mat4 u_modelViewProjectionMatrix;\n"
"};\n"
"\n"
"layout(location = 0) out vec2 var_TexDiffuse;\n"
"\n"
"void main(void)\n"
"{\n"
"    var_TexDiffuse = (u_textureMatrix * attr_TexCoord).xy;\n"
"\n"
"    gl_Position = attr_Vertex * u_modelViewProjectionMatrix;\n"
"}\n"
;
