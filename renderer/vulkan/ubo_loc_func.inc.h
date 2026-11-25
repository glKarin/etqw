#define UBOX_LOCATION_FUNC_DEFINE(stage, T) UBOX_LOCATION_DEFINE(stage, T);
#define UBO0_LOCATION_FUNC_DEFINE(T) UBOX_LOCATION_FUNC_DEFINE(vertex, T)
#define UBO1_LOCATION_FUNC_DEFINE(T) UBOX_LOCATION_FUNC_DEFINE(fragment, T)

#define UBO_VERTEX_PROC_BEGIN UBO0_LOCATION_FUNC_DEFINE
#define UBO_FRAGMENT_PROC_BEGIN UBO1_LOCATION_FUNC_DEFINE
#define UBO_VERTEX_PROC_END(x)
#define UBO_FRAGMENT_PROC_END(x)
#define UBO_MAT4_PROC(X)
#define UBO_VEC4_PROC(X)
#define UBO_FLOAT_PROC(X)
#define UBO_INT_N_PROC(X, N)
#define UBO_VEC4_N_PROC(X, N)
#define UBO_MAT4_N_PROC(X, N)

#include "ubo_prop_shader.inc.h"


typedef void (*VKGetProgramLocation_f)(VKGlobalUniformObjectLocation_t *ret);


#if 1
#define UBOX_LOCATION_PROC_ALIAS(stage, alias, original) \
    ID_INLINE void R_VK_GetUniformLocation_##stage##_##alias(VKGlobalUniformObjectLocation_t *ret) { \
        R_VK_GetUniformLocation_##stage##_##original(ret); \
    }
#else
#define UBOX_LOCATION_PROC_ALIAS(stage, alias, original) static const VKGetProgramLocation_f R_VK_GetUniformLocation_##stage##_##alias = &R_VK_GetUniformLocation_##stage##_##original;
#endif

#define UBO_VERTEX_PROC_ALIAS(alias, original) UBOX_LOCATION_PROC_ALIAS(vertex, alias, original)
#define UBO_FRAGMENT_PROC_ALIAS(alias, original) UBOX_LOCATION_PROC_ALIAS(fragment, alias, original)

#include "ubo_prop_alias.inc.h"


void R_VK_GetUniformLocation_all_clear(VKGlobalUniformObjectLocation_t *ret);
#define UBOX_LOCATION_NAME_ALL_CLEAR R_VK_GetUniformLocation_all_clear
