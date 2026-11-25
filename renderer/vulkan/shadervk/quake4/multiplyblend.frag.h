
// multiplyblend
GLSL_SHADER const char VK_MULTIPLYBLEND_FRAG[] =
"#version 450\n"
"#pragma shader_stage( fragment )\n"
"#extension GL_ARB_separate_shader_objects: enable\n"
"\n"
"layout(binding = 2, set = 1) uniform sampler2D Image;\n"
"\n"
"layout(location = 0) in vec2 texCoord;\n"
"layout(location = 1) in vec4 colorFragment;\n"
"\n"
"layout(location = 0) out vec4 _gl_FragColor;\n"
"\n"
"void main(void)\n"
"{\n"
"    _gl_FragColor = ( texture( Image, texCoord ) * colorFragment.a ) + ( vec4( 0.5, 0.5, 0.5, 0.5 ) * ( 1.0 - colorFragment.a ) );\n"
"}\n"
;
