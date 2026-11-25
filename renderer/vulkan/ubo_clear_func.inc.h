
#define UBOX_CLEAR_FUNC_DEFINE(stage, T) UBOX_CLEAR_DEFINE(stage, T);
#define UBO0_CLEAR_FUNC_DEFINE(T) UBOX_CLEAR_FUNC_DEFINE(vertex, T)
#define UBO1_CLEAR_FUNC_DEFINE(T) UBOX_CLEAR_FUNC_DEFINE(fragment, T)

#define UBO_VERTEX_PROC_BEGIN UBO0_CLEAR_FUNC_DEFINE
#define UBO_FRAGMENT_PROC_BEGIN UBO1_CLEAR_FUNC_DEFINE
#define UBO_VERTEX_PROC_END(x)
#define UBO_FRAGMENT_PROC_END(x)
#define UBO_MAT4_PROC(X)
#define UBO_VEC4_PROC(X)
#define UBO_FLOAT_PROC(X)
#define UBO_INT_N_PROC(X, N)
#define UBO_VEC4_N_PROC(X, N)
#define UBO_MAT4_N_PROC(X, N)

#include "ubo_prop_shader.inc.h"


typedef void (*VKClearUniformState_f)(void);


#if 1
#define UBOX_CLEAR_PROC_ALIAS(stage, alias, original) \
    ID_INLINE void R_VK_ClearUniformState_##stage##_##alias(void) { \
        R_VK_ClearUniformState_##stage##_##original(); \
    }
#else
#define UBOX_CLEAR_PROC_ALIAS(stage, alias, original) static const VKClearUniformState_f R_VK_ClearUniformState_##stage##_##alias = &R_VK_ClearUniformState_##stage##_##original;
#endif

#define UBO_VERTEX_PROC_ALIAS(alias, original) UBOX_CLEAR_PROC_ALIAS(vertex, alias, original)
#define UBO_FRAGMENT_PROC_ALIAS(alias, original) UBOX_CLEAR_PROC_ALIAS(fragment, alias, original)

#include "ubo_prop_alias.inc.h"
