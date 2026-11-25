#define UBOX_LOCATION_DECLARATION(stage, T) UBOX_LOCATION_DEFINE(stage, T) { \
	UBO_TYPEDEF(stage, T); \
	for(int i = 0; i < sizeof(VKGlobalUniformObjectLocation_t); i += sizeof(int)) { \
		*((int *)(((char *)(ret)) + i)) = -1; \
	}
#define UBO0_LOCATION_DECLARATION(T) UBOX_LOCATION_DECLARATION(vertex, T)
#define UBO1_LOCATION_DECLARATION(T) UBOX_LOCATION_DECLARATION(fragment, T)
#define END_UBO_LOCATION }

#define UBO_GLOBAL_LOCATION_VAL(name) *((int *)(((char *)(ret)) + (VKGLOBAL_PARM_OFFSET(name))))
#define UBO_LOCATION_TO_GLOBAL(name) UBO_GLOBAL_LOCATION_VAL(name) = SHADER_UBO_PARM_OFFSET(name); //printf(#name ": %d\n", UBO_GLOBAL_LOCATION_VAL(name));

#define UBO_GLOBAL_LOCATION_N_VAL(name, n) *((int *)(((char *)(ret)) + (VKGLOBAL_PARMS_OFFSET(name, n))))
#define UBO_LOCATION_TO_GLOBAL_N(name, n) UBO_GLOBAL_LOCATION_N_VAL(name, n) = SHADER_UBO_PARM_OFFSET_N(name, n);

#define UBO_VERTEX_PROC_BEGIN UBO0_LOCATION_DECLARATION
#define UBO_FRAGMENT_PROC_BEGIN UBO1_LOCATION_DECLARATION
#define UBO_VERTEX_PROC_END(x) END_UBO_LOCATION
#define UBO_FRAGMENT_PROC_END(x) END_UBO_LOCATION
#define UBO_MAT4_PROC UBO_LOCATION_TO_GLOBAL
#define UBO_VEC4_PROC UBO_LOCATION_TO_GLOBAL
#define UBO_FLOAT_PROC UBO_LOCATION_TO_GLOBAL
#define UBO_INT_N_PROC UBO_LOCATION_TO_GLOBAL_N
#define UBO_VEC4_N_PROC UBO_LOCATION_TO_GLOBAL_N
#define UBO_MAT4_N_PROC(X, N) UBO_LOCATION_TO_GLOBAL(X)

#include "ubo_prop_shader.inc.h"

void R_VK_GetUniformLocation_all_clear(VKGlobalUniformObjectLocation_t *ret)
{
	for(int i = 0; i < sizeof(VKGlobalUniformObjectLocation_t); i += sizeof(int)) {
		*((int *)(((char *)(ret)) + i)) = -1;
	}
}

