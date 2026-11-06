
// shadow
GLSL_SHADER const char VK_SHADOW_VERT[] =
"#version 450\n"
"#pragma shader_stage( vertex )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(location = 0) in vec4 attr_Vertex;\n"
"\n"
"layout(binding = 0) uniform UniformBufferObject {\n"
"    mat4 u_modelViewProjectionMatrix;\n"
"    vec4 u_glColor;\n"
"    vec4 u_lightOrigin;\n"
"};\n"
"\n"
"layout(location = 0) out vec4 var_Color;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_Position = u_modelViewProjectionMatrix * (attr_Vertex.w * u_lightOrigin + attr_Vertex - u_lightOrigin);\n"
"\n"
"    var_Color = u_glColor;\n"
"}\n"
;
