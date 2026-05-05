
// occlusion testing
GLSL_SHADER const char ES3_OCCLUSIONTEST_FRAG[] =
"#version 300 es\n"
"//#pragma optimize(off)\n"
"\n"
"precision mediump float;\n"
"\n"
"//#define _DEBUG\n"
"#ifdef _DEBUG\n"
"out vec4 _gl_FragColor;\n"
"#endif\n"
"\n"
"void main(void)\n"
"{\n"
"#ifdef _DEBUG\n"
"    _gl_FragColor = vec4(1.0);\n"
"#endif\n"
"}\n"
;

