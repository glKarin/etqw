
#define UBO_DECLARATION(stage, type) typedef struct stage##UniformBufferObject_##type {
#define END_UBO(stage, type) } UBO_DEFINE(stage, type);

#define VERTEX_UBO_DECLARATION(type) UBO_DECLARATION(vertex, type)
#define END_VERTEX_UBO(type) END_UBO(vertex, type)

#define FRAGMENT_UBO_DECLARATION(type) UBO_DECLARATION(fragment, type)
#define END_FRAGMENT_UBO(type) END_UBO(fragment, type)

#define layout_binding_0_uniform VERTEX_UBO_DECLARATION
#define end_layout_binding_0_uniform END_VERTEX_UBO
#define layout_binding_1_uniform FRAGMENT_UBO_DECLARATION
#define end_layout_binding_1_uniform END_FRAGMENT_UBO


#define UBO_VERTEX_PROC_BEGIN layout_binding_0_uniform
#define UBO_FRAGMENT_PROC_BEGIN layout_binding_1_uniform
#define UBO_VERTEX_PROC_END end_layout_binding_0_uniform
#define UBO_FRAGMENT_PROC_END end_layout_binding_1_uniform
#define UBO_MAT4_PROC(X) mat4 X
#define UBO_VEC4_PROC(X) vec4 X
#define UBO_FLOAT_PROC(X) float X
#define UBO_INT_N_PROC(X, N) int X##N
#define UBO_VEC4_N_PROC(X, N) vec4 X##N
#define UBO_MAT4_N_PROC(X, N) mat4 X[N]

#include "ubo_prop_shader.inc.h"


#define UBO_ALIAS_DEFINE(stage, alias, type) typedef stage##UniformBufferObject_##type##_t stage##UniformBufferObject_##alias##_t;

#define UBO_VERTEX_PROC_ALIAS(alias, type) UBO_ALIAS_DEFINE(vertex, alias, type)
#define UBO_FRAGMENT_PROC_ALIAS(alias, type) UBO_ALIAS_DEFINE(fragment, alias, type)


#include "ubo_prop_alias.inc.h"
