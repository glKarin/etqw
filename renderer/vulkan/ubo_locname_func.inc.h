#define UBOX_LOC_NAME_FUNC_DEFINE(stage, T) UBOX_LOC_NAME_DEFINE(stage, T);
#define UBO0_LOC_NAME_FUNC_DEFINE(T) UBOX_LOC_NAME_FUNC_DEFINE(vertex, T)
#define UBO1_LOC_NAME_FUNC_DEFINE(T) UBOX_LOC_NAME_FUNC_DEFINE(fragment, T)

#define UBO_VERTEX_PROC_BEGIN UBO0_LOC_NAME_FUNC_DEFINE
#define UBO_FRAGMENT_PROC_BEGIN UBO1_LOC_NAME_FUNC_DEFINE
#define UBO_VERTEX_PROC_END(x)
#define UBO_FRAGMENT_PROC_END(x)
#define UBO_MAT4_PROC(X)
#define UBO_VEC4_PROC(X)
#define UBO_FLOAT_PROC(X)
#define UBO_INT_N_PROC(X, N)
#define UBO_VEC4_N_PROC(X, N)
#define UBO_MAT4_N_PROC(X, N)

#include "ubo_prop_shader.inc.h"


typedef int (*VKGetUniformOffsetByName_f)(const char *name, int index);


#if 1
#define UBOX_LOC_NAME_PROC_ALIAS(stage, alias, original) \
    ID_INLINE void R_VK_GetUniformOffsetByName_##stage##_##alias(const char *name, int index) { \
        R_VK_GetUniformOffsetByName_##stage##_##original(name, index); \
    }
#else
#define UBOX_LOC_NAME_PROC_ALIAS(stage, alias, original) static const VKGetUniformOffsetByName_f R_VK_GetUniformOffsetByName_##stage##_##alias = &R_VK_GetUniformOffsetByName_##stage##_##original;
#endif

#define UBO_VERTEX_PROC_ALIAS(alias, original) UBOX_LOC_NAME_PROC_ALIAS(vertex, alias, original)
#define UBO_FRAGMENT_PROC_ALIAS(alias, original) UBOX_LOC_NAME_PROC_ALIAS(fragment, alias, original)

#include "ubo_prop_alias.inc.h"

