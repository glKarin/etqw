
// occlusion testing
GLSL_SHADER const char OCCLUSIONTEST_FRAG[] =
"#version 100\n"
"//#pragma optimize(off)\n"
"\n"
"precision mediump float;\n"
"\n"
"//#define _DEBUG\n"
"#ifdef _DEBUG\n"
"#endif\n"
"\n"
"void main(void)\n"
"{\n"
"#ifdef _DEBUG\n"
"    gl_FragColor = vec4(1.0);\n"
"#endif\n"
"}\n"
;

