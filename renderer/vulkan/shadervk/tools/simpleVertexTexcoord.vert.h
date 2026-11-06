
// simple vertex/texcoord vertex shader
GLSL_SHADER const char VK_SIMPLE_VERTEX_TEXCOORD_VERT[] =
"#version 450\n"
"#pragma shader_stage( vertex )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(location = 8) in vec4 attr_TexCoord;\n"
"layout(location = 0) in vec4 attr_Vertex;\n"
"\n"
"layout(binding = 0) uniform UniformBufferObject {\n"
"    mat4 u_modelViewProjectionMatrix;\n"
"};\n"
"\n"
"layout(location = 0) out vec2 var_TexDiffuse;\n"
"\n"
"void main(void)\n"
"{\n"
"    var_TexDiffuse = attr_TexCoord.xy;\n"
"\n"
"    gl_Position = u_modelViewProjectionMatrix * attr_Vertex;\n"
"}\n"
;
