#define UBOX_LOC_NAME_DECLARATION(stage, T) UBOX_LOC_NAME_DEFINE(stage, T) { \
	UBO_TYPEDEF(stage, T);
#define UBO0_LOC_NAME_DECLARATION(T) UBOX_LOC_NAME_DECLARATION(vertex, T)
#define UBO1_LOC_NAME_DECLARATION(T) UBOX_LOC_NAME_DECLARATION(fragment, T)
#define END_UBO_LOC_NAME \
		return -1; \
	}

#ifdef snprintf
#undef snprintf
#endif

#define UBO_GET_SHADER_OFFSET_BY_NAME(x) \
	if(strcmp(name, #x) == 0) { \
		return SHADER_UBO_PARM_OFFSET(x); \
	}
	//printf(#name ": %d\n", UBO_GLOBAL_LOC_NAME_VAL(name));

#define UBO_GET_SHADER_OFFSET_BY_NAME_N(x, n)  \
	{ \
		char temp[128]; \
		snprintf(temp, sizeof(temp), #x "%d", index); \
		if(strcmp(temp, #x #n) == 0 && (n) == index) { \
			return SHADER_UBO_PARM_OFFSET_N(x, n); \
		} \
	}

#define UBO_VERTEX_PROC_BEGIN UBO0_LOC_NAME_DECLARATION
#define UBO_FRAGMENT_PROC_BEGIN UBO1_LOC_NAME_DECLARATION
#define UBO_VERTEX_PROC_END(x) END_UBO_LOC_NAME
#define UBO_FRAGMENT_PROC_END(x) END_UBO_LOC_NAME
#define UBO_MAT4_PROC UBO_GET_SHADER_OFFSET_BY_NAME
#define UBO_VEC4_PROC UBO_GET_SHADER_OFFSET_BY_NAME
#define UBO_FLOAT_PROC UBO_GET_SHADER_OFFSET_BY_NAME
#define UBO_INT_N_PROC UBO_GET_SHADER_OFFSET_BY_NAME_N
#define UBO_VEC4_N_PROC UBO_GET_SHADER_OFFSET_BY_NAME_N
#define UBO_MAT4_N_PROC(X, N) UBO_GET_SHADER_OFFSET_BY_NAME(X)


#include "ubo_prop_shader.inc.h"

